export const CONFIG_FILES = [
  "config.json",
  "race.json",
  "job.json",
  "skill.json",
  "effect.json",
  "buff.json",
] as const;

export type ConfigFileName = (typeof CONFIG_FILES)[number];

export interface ConfigVersionResponse {
  config_version: number;
  source: string;
}

const API_BASE_URL = import.meta.env.VITE_API_BASE_URL ?? "";

async function requestJson<TResponse>(path: string): Promise<TResponse> {
  const response = await fetch(`${API_BASE_URL}${path}`, {
    headers: {
      "Content-Type": "application/json",
    },
  });

  if (!response.ok) {
    throw new Error(`HTTP ${response.status}: ${response.statusText}`);
  }

  return response.json() as Promise<TResponse>;
}

export function getConfigVersion(): Promise<ConfigVersionResponse> {
  return requestJson<ConfigVersionResponse>("/api/config/version");
}

export function getConfigFile(configName: ConfigFileName): Promise<unknown> {
  return requestJson<unknown>(`/api/config/${encodeURIComponent(configName)}`);
}
