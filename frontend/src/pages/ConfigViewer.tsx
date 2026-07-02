import { useEffect, useState } from "react";

import { Button } from "@/components/ui/button";
import {
  CONFIG_FILES,
  getConfigFile,
  getConfigVersion,
  type ConfigFileName,
  type ConfigVersionResponse,
} from "@/services/configClient";

function formatJson(value: unknown): string {
  return JSON.stringify(value, null, 2);
}

export function ConfigViewer() {
  const [selectedConfig, setSelectedConfig] =
    useState<ConfigFileName>("config.json");
  const [configData, setConfigData] = useState<unknown>(null);
  const [version, setVersion] = useState<ConfigVersionResponse | null>(null);
  const [isLoading, setIsLoading] = useState(false);
  const [errorMessage, setErrorMessage] = useState<string | null>(null);

  async function loadConfig(configName = selectedConfig) {
    setIsLoading(true);
    setErrorMessage(null);

    try {
      const [versionResult, configResult] = await Promise.all([
        getConfigVersion(),
        getConfigFile(configName),
      ]);
      setVersion(versionResult);
      setConfigData(configResult);
    } catch (error) {
      setErrorMessage(error instanceof Error ? error.message : "读取配置失败");
      setConfigData(null);
    } finally {
      setIsLoading(false);
    }
  }

  function selectConfig(configName: ConfigFileName) {
    setSelectedConfig(configName);
    void loadConfig(configName);
  }

  useEffect(() => {
    void loadConfig();
  }, []);

  return (
    <main className="mx-auto min-h-screen max-w-7xl px-6 py-8">
      <header className="mb-6 flex flex-col gap-3 md:flex-row md:items-end md:justify-between">
        <div>
          <p className="text-sm font-medium text-muted-foreground">
            Config Portal
          </p>
          <h1 className="text-3xl font-bold tracking-tight">配置查看器</h1>
          <p className="mt-2 text-muted-foreground">
            读取后端白名单配置文件，并查看当前配置版本。
          </p>
        </div>
        <Button onClick={() => void loadConfig()} disabled={isLoading}>
          {isLoading ? "刷新中..." : "刷新"}
        </Button>
      </header>

      {errorMessage ? (
        <section className="mb-6 rounded-lg border border-destructive/40 bg-destructive/10 p-4 text-sm text-destructive">
          {errorMessage}
        </section>
      ) : null}

      <section className="grid gap-6 lg:grid-cols-[280px_minmax(0,1fr)]">
        <aside className="rounded-xl border bg-card p-4 text-card-foreground shadow-sm">
          <h2 className="mb-3 text-lg font-semibold">Config Files</h2>
          <div className="space-y-2">
            {CONFIG_FILES.map((configName) => (
              <button
                key={configName}
                className={`w-full rounded-md px-3 py-2 text-left font-mono text-sm transition-colors ${
                  selectedConfig === configName
                    ? "bg-primary text-primary-foreground"
                    : "hover:bg-accent hover:text-accent-foreground"
                }`}
                type="button"
                onClick={() => selectConfig(configName)}
              >
                {configName}
              </button>
            ))}
          </div>

          <dl className="mt-6 space-y-3 text-sm">
            <div>
              <dt className="font-medium text-foreground">版本</dt>
              <dd className="text-muted-foreground">
                {version ? version.config_version : "-"}
              </dd>
            </div>
            <div>
              <dt className="font-medium text-foreground">来源</dt>
              <dd className="text-muted-foreground">
                {version ? version.source : "-"}
              </dd>
            </div>
          </dl>
        </aside>

        <section className="rounded-xl border bg-card p-4 text-card-foreground shadow-sm">
          <div className="mb-3 flex items-center justify-between gap-3">
            <h2 className="text-lg font-semibold">{selectedConfig}</h2>
            {isLoading ? (
              <span className="text-sm text-muted-foreground">读取中...</span>
            ) : null}
          </div>
          <pre className="min-h-[560px] overflow-auto rounded-md bg-secondary p-4 text-sm">
            {configData ? formatJson(configData) : "暂无配置数据"}
          </pre>
        </section>
      </section>
    </main>
  );
}
