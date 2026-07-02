import { useEffect, useState } from "react";

import { Button } from "@/components/ui/button";
import { getSave, listSaves } from "@/services/saveClient";
import type { SaveRecord, SaveSummary } from "@/types/save";

function formatDate(value: string): string {
  return new Intl.DateTimeFormat("zh-CN", {
    dateStyle: "medium",
    timeStyle: "short",
  }).format(new Date(value));
}

function formatJson(value: unknown): string {
  return JSON.stringify(value, null, 2);
}

export function SaveList() {
  const [saves, setSaves] = useState<SaveSummary[]>([]);
  const [selectedSave, setSelectedSave] = useState<SaveRecord | null>(null);
  const [isLoadingList, setIsLoadingList] = useState(false);
  const [loadingSaveId, setLoadingSaveId] = useState<string | null>(null);
  const [errorMessage, setErrorMessage] = useState<string | null>(null);

  async function loadSaves() {
    setIsLoadingList(true);
    setErrorMessage(null);

    try {
      const result = await listSaves();
      if (!result.ok) {
        setErrorMessage(result.error?.message ?? "读取存档列表失败");
        setSaves([]);
        setSelectedSave(null);
        return;
      }

      setSaves(result.saves);
      if (
        !result.saves.some((save) => save.save_id === selectedSave?.save_id)
      ) {
        setSelectedSave(null);
      }
    } catch (error) {
      setErrorMessage(
        error instanceof Error ? error.message : "读取存档列表失败",
      );
      setSaves([]);
      setSelectedSave(null);
    } finally {
      setIsLoadingList(false);
    }
  }

  async function loadSaveDetail(saveId: string) {
    setLoadingSaveId(saveId);
    setErrorMessage(null);

    try {
      const result = await getSave(saveId);
      if (!result.ok || !result.save) {
        setErrorMessage(result.error?.message ?? "读取存档详情失败");
        setSelectedSave(null);
        return;
      }

      setSelectedSave(result.save);
    } catch (error) {
      setErrorMessage(
        error instanceof Error ? error.message : "读取存档详情失败",
      );
      setSelectedSave(null);
    } finally {
      setLoadingSaveId(null);
    }
  }

  useEffect(() => {
    void loadSaves();
  }, []);

  return (
    <main className="mx-auto min-h-screen max-w-7xl px-6 py-8">
      <header className="mb-6 flex flex-col gap-3 md:flex-row md:items-end md:justify-between">
        <div>
          <p className="text-sm font-medium text-muted-foreground">
            Save Portal
          </p>
          <h1 className="text-3xl font-bold tracking-tight">存档列表</h1>
          <p className="mt-2 text-muted-foreground">
            查看后端本地 JSON 存档摘要，并按需读取单个存档详情。
          </p>
        </div>
        <Button onClick={loadSaves} disabled={isLoadingList}>
          {isLoadingList ? "刷新中..." : "刷新"}
        </Button>
      </header>

      {errorMessage ? (
        <section className="mb-6 rounded-lg border border-destructive/40 bg-destructive/10 p-4 text-sm text-destructive">
          {errorMessage}
        </section>
      ) : null}

      <section className="grid gap-6 lg:grid-cols-[minmax(0,1fr)_minmax(360px,0.85fr)]">
        <div className="rounded-xl border bg-card p-4 text-card-foreground shadow-sm">
          <div className="mb-4 flex items-center justify-between gap-3">
            <h2 className="text-lg font-semibold">Saves</h2>
            <span className="text-sm text-muted-foreground">
              {saves.length} 条
            </span>
          </div>

          {isLoadingList ? (
            <p className="text-sm text-muted-foreground">正在读取存档...</p>
          ) : saves.length === 0 ? (
            <p className="text-sm text-muted-foreground">暂无存档</p>
          ) : (
            <ul className="space-y-3">
              {saves.map((save) => (
                <li
                  key={save.save_id}
                  className="rounded-md border bg-background p-4"
                >
                  <div className="flex flex-col gap-3 md:flex-row md:items-start md:justify-between">
                    <div className="min-w-0">
                      <h3 className="truncate text-base font-semibold">
                        {save.name}
                      </h3>
                      <p className="mt-1 break-all font-mono text-xs text-muted-foreground">
                        {save.save_id}
                      </p>
                      <dl className="mt-3 grid gap-2 text-sm text-muted-foreground sm:grid-cols-2">
                        <div>
                          <dt className="font-medium text-foreground">更新</dt>
                          <dd>{formatDate(save.updated_at)}</dd>
                        </div>
                        <div>
                          <dt className="font-medium text-foreground">
                            格式版本
                          </dt>
                          <dd>{save.save_format_version}</dd>
                        </div>
                      </dl>
                    </div>
                    <Button
                      variant="outline"
                      onClick={() => void loadSaveDetail(save.save_id)}
                      disabled={loadingSaveId === save.save_id}
                    >
                      {loadingSaveId === save.save_id ? "读取中..." : "查看"}
                    </Button>
                  </div>
                </li>
              ))}
            </ul>
          )}
        </div>

        <aside className="rounded-xl border bg-card p-4 text-card-foreground shadow-sm">
          <h2 className="mb-3 text-lg font-semibold">Save Detail</h2>
          <pre className="min-h-[520px] overflow-auto rounded-md bg-secondary p-4 text-sm">
            {selectedSave ? formatJson(selectedSave) : "选择一个存档查看详情"}
          </pre>
        </aside>
      </section>
    </main>
  );
}
