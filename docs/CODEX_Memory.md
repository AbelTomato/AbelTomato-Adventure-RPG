# CODEX Memory

Last updated: 2026-05-25

This file records my current understanding of the project after reading `docs/game_design_spec.md`, `docs/inventory_design.md`, `data/`, `include/`, `src/`, `README.md`, `CMakeLists.txt`, and the supporting docs.

## Project Identity

- Project name: `TomatoHeroAbel`, also described in README as `爱吃番茄的勇者Abel`.
- Current implementation target: C++ console/text RPG prototype using CMake, MinGW, and bundled `nlohmann/json`.
- Design ambition: data-driven RPG core with attributes, race/job data, skills, effects, buffs, item/inventory data, local JSON persistence, grid-based exploration, and semi-real-time turn sequencing.
- The design spec is larger than the current executable. The codebase is currently a systems/data prototype; `src/main.cpp` is effectively empty.

## Design Spec Summary

- Core attributes: strength, physique, dexterity, esthesia, bewitchment, willpower, life_growth, magic_growth, speed, luck.
- Attribute model separates base attributes, derived attributes, modifiers, final attributes, and current runtime state by `Attr::ID` index ranges.
- Modifiers use two shapes:
  - `Mod { pct, flat }` for additive/multiplicative stat changes.
  - Raw `double` additive rate modifiers for crit, block, damage reduction, ignore defense, and damage increase.
- Gameplay model:
  - Semi-real-time turn-based combat based on speed ratio.
  - "Player does not act, world is still" interaction rhythm.
  - 2D square grid map, eight-direction movement, grid-based collision/event/effect judgement.
  - World loop: exploration, resource gathering, dynamic encounters, staged progression.
  - Local snapshot persistence through JSON.
- Data import rule: enum values should be represented as strings in JSON.
- Data ID ranges in the spec matter. Current sample data does not fully obey them yet.

## Repository Layout

- `data/`: JSON samples/configs for config, race, job, skill, effect, buff. Item category JSON files are not present yet.
- `docs/`: design documentation. `docs/CODEX_Memory.md` is the project memory file.
- `include/definitions/`: enums and core value structs.
- `include/data/`: serializable data records such as `RaceData`, `JobData`, `SkillData`, `EffectData`, `BuffData`, `ItemData`.
- `include/components/`: reusable gameplay components such as `BehaviorComponent` and `StatBonusComponent`.
- `include/systems/`: config loading, data repositories, attribute manager, stat calculator.
- `include/inventory/`: inventory data/UI-adjacent headers.
- `src/systems/`: implementations for config, data loading, attribute calculation.
- `src/InventoryLogic/`: inventory implementation and UI placeholders.
- `third_party/nlohmann/json.hpp`: bundled JSON library.

## Runtime Architecture

### Definitions Layer

- `Attr::ID` in `include/definitions/attribute_id.hpp` is the central numeric contract for attribute lookup.
- `Attributes` and `Mod` live in `include/definitions/core_attributes.hpp`.
- `DerivedStats` and `BonusStats` mirror the spec's stat containers, but the active implementation mostly uses indexed arrays in `AttributeManager`.
- JSON enum adapters exist for damage, behavior trigger/target, effect, item type/rarity, skill/cost/shape/target filter.

### Data Layer

- All data records inherit or embed the lightweight `BaseData { id, name, description }`.
- `RaceData` and `JobData` contain `Attributes base_attr` and skill ID lists.
- `SkillData` currently expects `display_effect` and `BehaviorComponent behaviors`.
- `BehaviorComponent` contains a vector of `PassiveAction`.
- `PassiveAction` expects:
  - `trigger_type`
  - `chance`
  - `conditions`
  - `action_type`
  - `action_id`
  - `params`
- `EffectData` maps `func_name` plus `logic_address`, suggesting a future function registry/effect execution pipeline.
- `ItemData` supports item type, weight, stackability, default rarity, optional usage count, optional base/random affix pools, and optional behavior component.

### Config And Data Loading

- `ConfigManager` is a singleton with:
  - `Config configs`
  - `CalcCoefficients cache`
  - `load_all_configs(path)`
  - `sync_cache(conf)`
- `Config` stores nested maps and exposes `get(category, key)`.
- `DataRepository<T>` loads JSON arrays into `id -> data` and `name -> id` maps.
- `DataManager` is a singleton holding repositories for races, skills, effects, buffs, jobs, and item categories.
- `DataManager::load_all_data()` currently expects separate item files:
  - `weapon.json`
  - `armor.json`
  - `accessory.json`
  - `comsumable.json`
  - `material.json`
  - `quest.json`

### Attribute System

- `AttributeManager` stores all numeric values in `std::array<double, Attr::MaxCount> _data`.
- It stores `Mod` entries in `std::array<Mod, Attr::MaxCount> _mods`.
- Recalculation is lazy through `_is_dirty`.
- Recalculation order:
  1. `calculate_primary_finals()`
  2. `StatCalculator::compute_derived_bases(_data, G_Cache)`
  3. `apply_secondary_mods()`
- `StatCalculator` implements spec formulas for HP, MP, SP, attack, evasion, hit, defense, ignore defense, and crit rate.
- Some spec values are not fully computed/clamped yet: block rate, damage reductions, true reduction, crit damage, damage increase, and battle caps.

### Inventory System

- `docs/inventory_design.md` describes the intended split:
  - `Inventory` / `InventoryComponent`: data layer.
  - `InventoryMenu`: UI manager.
  - `InventorySlot`: UI child slots.
  - `InventoryToolTip`: item description UI.
  - `InventoryDragDropOperation`: drag/drop payload.
- `Inventory` currently owns:
  - `std::vector<ItemStack> items`
  - `current_capacity`
  - `max_stack`
  - single `OnSlotUpdatedDelegate`
- `ItemStack` is `{ int count, std::optional<ItemData> data }`.
- `Inventory::add_item()` handles stackable and non-stackable item insertion and broadcasts changed slot indices.
- `Inventory::swap_item()` swaps slots and broadcasts both.
- `Inventory::use_item()` is a switch placeholder by `ItemType`.
- `InventoryMenu` owns UI slots as `std::unique_ptr<InventorySlot>` and keeps a `std::weak_ptr<Inventory>`.
- `InventorySlot::refresh_UI()` pulls data from the inventory weak reference and is waiting for real graphical UI.

## Current Data State

- `data/race.json` mostly matches `RaceData`.
- `data/job.json` mostly matches `JobData`.
- `data/effect.json` matches the current `EffectData` shape.
- `data/config.json` does not currently match `ConfigManager::sync_cache()`:
  - JSON uses `Settings`, but `Config` expects `settings`.
  - JSON uses `Attr`, `Growth`, `Battle`, but code calls `conf.get("attr", ...)`.
  - Several key names differ by capitalization, such as `HpPerStrength` vs `HPPerStrength`.
  - Growth keys live under `Growth`, but `sync_cache()` reads them from `attr`.
- `data/skill.json` is partly old schema:
  - First skill uses `actions_id`, while `PassiveAction` expects `action_id`.
  - Later skills use `cost`, `cooldown`, `target_type`, and `effect_triggers`, but `SkillData` expects `behaviors`.
  - Several skill IDs such as `5001` are outside the spec's stated skill ID range.
- `data/buff.json` is old schema relative to `BuffData`; current `BuffData` expects `behaviors`.
- Item JSON files expected by `DataManager::load_all_data()` are not present.

## Verified Build State

- `cmake -S . -B build` failed because CMake selected `NMake Makefiles` and `nmake` is not installed in this environment.
- `cmake -S . -B build-ninja -G Ninja -DCMAKE_C_COMPILER=...gcc.exe -DCMAKE_CXX_COMPILER=...g++.exe` configured successfully with MinGW GCC 13.1.0.
- `cmake --build build-ninja` failed before linking. Key compile blockers:
  - `std::map::contains` is used in `DataRepository<T>::load()`, but `CMakeLists.txt` sets C++17. `contains` requires C++20, or this should be rewritten using `find`.
  - `ItemData` uses `std::optional<T>` fields with `NLOHMANN_DEFINE_TYPE_INTRUSIVE`, but no optional serializer is provided for the bundled nlohmann/json setup.
- README says C++11, CMake says C++17, and code currently uses at least C++17 features plus one C++20 API.

## Important Risks And Bugs

- Do not assume the JSON files currently load. Data schema alignment is the biggest immediate blocker.
- `Inventory::remove_item()` has a logic bug:
  - It decrements `left_to_remove` while discovering candidate slots, then reuses the already-decremented value during actual removal.
  - This can remove the wrong amount or even increase counts if the value is negative.
  - It also dereferences `I.data->id` without checking `I.data.has_value()`.
- `InventoryToolTip::hide_tool_tip()` is declared as a member but implemented as a free function in `inventory_tool_tip.cpp`.
- `InventoryDragDropOperation` is effectively empty.
- `InventoryMenu::toggle_menu()` and `Inventory::update_inventory()` are declared but not implemented.
- `OnSlotUpdatedDelegate` is single-cast. That is fine for one menu, but future multiple observers will overwrite each other.
- `AttributeManager::calculate_primary_finals()` relies on numeric offset constants (`DerivedCount`, `ModCount`) matching the enum layout exactly.
- `AttributeManager::apply_secondary_mods()` has no battle cap enforcement yet.
- `ConfigManager::sync_cache()` must be called after `load_all_configs()` before any derived stat calculation, otherwise `G_Cache` is default zeroed.
- `include/definitions/buff_type.hpp` is empty.
- `StatBonusComponent` uses `NLOHMANN_JSON_SERIALIZE_ENUM` on structs, which appears conceptually wrong and should be reviewed when that component is compiled/used.

## Architectural Direction To Preserve

- Keep the core game logic independent of UI/rendering. The best current boundary is:
  - data/config definitions
  - pure systems (`AttributeManager`, `StatCalculator`, future combat/effect/buff/grid systems)
  - inventory data operations
  - UI/menu/slot adapters
- Prefer JSON-driven content over hardcoded gameplay data.
- Treat `Attr::ID` as a central ABI-like contract. Changing values requires migrating data and calculation code together.
- Before adding new gameplay features, first stabilize:
  1. C++ standard/build configuration.
  2. JSON schemas and serializers.
  3. Config key names and cache synchronization.
  4. Unit tests for `StatCalculator`, `AttributeManager`, `DataRepository`, and `Inventory`.
- For future effect/combat work, the natural design is a registry-driven `EffectEngine`:
  - `SkillData`/`BuffData`/`ItemData`
  - `BehaviorComponent`
  - `PassiveAction`
  - `ConditionData`
  - `EffectData { func_name, logic_address }`
  - registered C++ effect functions operating on an explicit context.

## Local Working Tree Notes

- `include/definitions/core_attributes.hpp` was already modified in the worktree before this memory update. Do not overwrite it casually.
- Generated build directories may appear during verification; do not commit build artifacts.
