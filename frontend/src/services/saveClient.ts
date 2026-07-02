import type { SaveGetSingleResponse, SaveListResponse } from "../types/save";

const API_BASE_URL = import.meta.env.VITE_API_BASE_URL ?? "";

async function requestJson<TResponse>(
  path: string,
  init?: RequestInit,
): Promise<TResponse> {
  const response = await fetch(`${API_BASE_URL}${path}`, {
    headers: {
      "Content-Type": "application/json",
      ...init?.headers,
    },
    ...init,
  });

  if (!response.ok) {
    throw new Error(`HTTP ${response.status}: ${response.statusText}`);
  }

  return response.json() as Promise<TResponse>;
}

export function listSaves(): Promise<SaveListResponse> {
  return requestJson<SaveListResponse>("/api/saves");
}

export function getSave(saveId: string): Promise<SaveGetSingleResponse> {
  return requestJson<SaveGetSingleResponse>(
    `/api/saves/${encodeURIComponent(saveId)}`,
  );
}
