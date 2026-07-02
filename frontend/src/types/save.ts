import type { GameError, GameState } from "./game";

export interface SaveRecord {
  save_id: string;
  name: string;
  created_at: string;
  updated_at: string;
  save_format_version: number;
  state: GameState;
  meta: Record<string, unknown>;
}

export interface SaveSummary {
  save_id: string;
  name: string;
  created_at: string;
  updated_at: string;
  save_format_version: number;
}

export interface SaveCreateRequest {
  contract_version: number;
  request_id: string;
  name: string;
  state: GameState;
  meta: Record<string, unknown>;
}

export interface SaveUpdateRequest {
  contract_version: number;
  request_id: string;
  name: string;
  state: GameState;
  meta: Record<string, unknown>;
}

export interface SaveCreateResponse {
  ok: boolean;
  contract_version: number;
  request_id: string;
  save: SaveRecord | null;
  error: GameError | null;
}

export interface SaveListResponse {
  ok: boolean;
  contract_version: number;
  request_id: string | null;
  saves: SaveSummary[];
  error: GameError | null;
}

export interface SaveGetSingleResponse {
  ok: boolean;
  contract_version: number;
  request_id: string | null;
  save: SaveRecord | null;
  error: GameError | null;
}

export interface SaveUpdateResponse {
  ok: boolean;
  contract_version: number;
  request_id: string;
  save: SaveRecord | null;
  error: GameError | null;
}

export interface SaveDeleteResponse {
  ok: boolean;
  contract_version: number;
  request_id: string | null;
  deleted: boolean;
  save_id: string;
  error: GameError | null;
}
