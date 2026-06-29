extends Control

const API_URL := "http://127.0.0.1:8000/api/game/action"

@onready var attack_button: Button = $AttackButton
@onready var log_text: RichTextLabel = $LogText
@onready var http_request: HTTPRequest = $HTTPRequest

func _ready() -> void:
	print("ApiTestScene ready")

	attack_button.position = Vector2(40, 40)
	attack_button.size = Vector2(180, 50)
	attack_button.text = "Attack"

	log_text.position = Vector2(40, 110)
	log_text.size = Vector2(700, 360)
	log_text.text = "Ready: click Attack to send request later."

	attack_button.pressed.connect(_on_attack_pressed)
	http_request.request_completed.connect(_on_request_completed)

func _on_attack_pressed() -> void:
	log_text.text = "Sending attack request..."
	print("Sending attack request...")

	var request_body := {
		"contract_version": 1,
		"request_id": "req_attack_001",
		"action": {
			"type": "attack",
			"actor_id": "player_1",
			"target_id": "slime_1",
			"params": {}
		},
		"state": {
			"turn": {
				"round": 1,
				"active_entity_id": "player_1"
			},
			"entities": [
				{
					"id": "player_1",
					"name": "Abel",
					"type": "player",
					"attrs": {
						"hp": 100,
						"max_hp": 100,
						"physical_attack": 20,
						"physical_defense": 5
					},
					"status": {
						"alive": true
					}
				},
				{
					"id": "slime_1",
					"name": "Slime",
					"type": "monster",
					"attrs": {
						"hp": 50,
						"max_hp": 50,
						"physical_attack": 8,
						"physical_defense": 2
					},
					"status": {
						"alive": true
					}
				}
			]
		},
		"meta": {
			"client": "godot",
			"debug": true
		}
	}
	var json_body := JSON.stringify(request_body)
	var headers := ["Content-Type: application/json; charset=utf-8"]

	var error := http_request.request(
		API_URL,
		headers,
		HTTPClient.METHOD_POST,
		json_body
	)
	if error != OK:
		log_text.text = "Failed to Send the Request. Error Code: %s " % error
		print("Failed to send request: ", error)

func _on_request_completed(
		result: int,
		response_code: int,
		headers: PackedStringArray,
		body: PackedByteArray
) ->void :
	var response_text := body.get_string_from_utf8()
	print("HTTP result: ", result)
	print("HTTP status: ", response_code)
	print("Response body: ", response_text)

	var parsed = JSON.parse_string(response_text)
	if parsed == null:
		log_text.text = "Failed to parse response JSON."
		return

	if not parsed.get("ok",false):
		var error = parsed.get("error",{})
		log_text.text = "API error : %s\n%s" % [
			error.get("code", "UNKNOWN"),
			error.get("message", "")
		]
		return

	var lines: Array[String] = []
	lines.append("Attack response OK")
	lines.append("Events")

	for event in parsed.get("events", []):
		var event_type := str(event.get("type", "unknown"))
		var seq := str(event.get("seq", "?"))
		match event_type:
			"attack_started":
				lines.append("%s: %s attacks %s" %
				[
					seq,
					event.get("source_id", "?"),
					event.get("target_id", "?")
				])
			"damage":
				lines.append("%s :%s deals %s %s damage to %s" %
				[
					seq,
					event.get("source_id", "?"),
					event.get("value","?"),
					event.get("damage_type","unknown"),
					event.get("target_id","?")
				])
			"hp_changed":
				lines.append("%s: %s HP %s -> %s" %
				[
					seq,
					event.get("entity_id", "?"),
					event.get("from", "?"),
					event.get("to", "?")
				])
			"death":
				lines.append("%s: %s died" %
				[
					seq,
					event.get("entity_id", "?")
				])
			_:
				lines.append("%s: %s" % [seq, event_type])

	lines.append("")
	lines.append("Final state: ")
	var entities : Array =parsed.get("state",{}).get("entities",[])
	for entity in entities:
		var attrs: Dictionary = entity.get("attrs", {})
		lines.append("%s HP: %s / %s" % [
			entity.get("id", "?"),
			attrs.get("hp", "?"),
			attrs.get("max_hp", "?")
		])

	log_text.text = "\n".join(lines)
