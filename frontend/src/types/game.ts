export interface GameAction {
  type: string;
  actor_id: string;
  target_id?: string | null;
  params: Record<string, unknown>;
}

export interface TurnState {
  round: number;
  active_entity_id: string;
}

export interface EntityAttrs {
  hp: number;
  max_hp: number;
  physical_attack: number;
  physical_defense: number;
}

export interface EntityStatus {
  alive: boolean;
}

export interface EntityState {
  id: string;
  name: string;
  type: string;
  attrs: EntityAttrs;
  status: EntityStatus;
}

export interface GameState {
  turn: TurnState;
  entities: EntityState[];
}

export interface GameActionRequest {
  contract_version: number;
  request_id: string;
  action: GameAction;
  state: GameState;
  meta: Record<string, unknown>;
}

export interface GameEvent {
  seq: number;
  type: string;
  [key: string]: unknown;
}

export interface GameError {
  code: string;
  message: string;
  details?: Record<string, unknown>;
}

export interface GameActionResponse {
  ok: boolean;
  contract_version: number;
  request_id: string;
  state: GameState | null;
  events: GameEvent[];
  error: GameError | null;
}
