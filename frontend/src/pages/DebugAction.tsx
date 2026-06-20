import { useEffect, useMemo, useState } from "react";

import { Button } from "@/components/ui/button";
import { getExampleAction, postGameAction } from "@/services/apiClient";
import type { GameActionRequest, GameActionResponse } from "@/types/game";

function formatJson(value: unknown): string {
  return JSON.stringify(value, null, 2);
}

function parseRequestJson(value: string): GameActionRequest {
  return JSON.parse(value) as GameActionRequest;
}

export function DebugAction() {
  const [requestText, setRequestText] = useState("");
  const [response, setResponse] = useState<GameActionResponse | null>(null);
  const [errorMessage, setErrorMessage] = useState<string | null>(null);
  const [isLoadingExample, setIsLoadingExample] = useState(false);
  const [isSending, setIsSending] = useState(false);

  const parsedRequest = useMemo(() => {
    if (!requestText.trim()) {
      return null;
    }

    try {
      return parseRequestJson(requestText);
    } catch {
      return null;
    }
  }, [requestText]);

  async function loadExampleAction() {
    setIsLoadingExample(true);
    setErrorMessage(null);

    try {
      const example = await getExampleAction();
      setRequestText(formatJson(example));
      setResponse(null);
    } catch (error) {
      setErrorMessage(
        error instanceof Error ? error.message : "加载示例请求失败",
      );
    } finally {
      setIsLoadingExample(false);
    }
  }

  async function sendAction() {
    setErrorMessage(null);

    if (!parsedRequest) {
      setErrorMessage("Request JSON 格式不合法");
      return;
    }

    setIsSending(true);

    try {
      const result = await postGameAction(parsedRequest);
      setResponse(result);
    } catch (error) {
      setErrorMessage(
        error instanceof Error ? error.message : "发送 action 失败",
      );
    } finally {
      setIsSending(false);
    }
  }

  useEffect(() => {
    void loadExampleAction();
  }, []);

  return (
    <main className="mx-auto min-h-screen max-w-7xl px-6 py-8">
      <header className="mb-6 flex flex-col gap-3 md:flex-row md:items-end md:justify-between">
        <div>
          <p className="text-sm font-medium text-muted-foreground">
            React Debug Console
          </p>
          <h1 className="text-3xl font-bold tracking-tight">
            Game Action 调试台
          </h1>
          <p className="mt-2 text-muted-foreground">
            加载示例请求，编辑 JSON，发送到 FastAPI Mock Backend，并查看
            state、events 和 error。
          </p>
        </div>
        <div className="flex gap-3">
          <Button
            variant="outline"
            onClick={loadExampleAction}
            disabled={isLoadingExample}
          >
            {isLoadingExample ? "加载中..." : "加载示例"}
          </Button>
          <Button onClick={sendAction} disabled={isSending}>
            {isSending ? "发送中..." : "发送 Action"}
          </Button>
        </div>
      </header>

      {errorMessage ? (
        <section className="mb-6 rounded-lg border border-destructive/40 bg-destructive/10 p-4 text-sm text-destructive">
          {errorMessage}
        </section>
      ) : null}

      <section className="grid gap-6 lg:grid-cols-2">
        <div className="rounded-xl border bg-card p-4 text-card-foreground shadow-sm">
          <h2 className="mb-3 text-lg font-semibold">Request JSON</h2>
          <textarea
            className="min-h-[560px] w-full resize-y rounded-md border bg-background p-4 font-mono text-sm outline-none focus:ring-2 focus:ring-ring"
            value={requestText}
            onChange={(event) => setRequestText(event.target.value)}
            spellCheck={false}
          />
        </div>

        <div className="flex flex-col gap-6">
          <section className="rounded-xl border bg-card p-4 text-card-foreground shadow-sm">
            <h2 className="mb-3 text-lg font-semibold">Response JSON</h2>
            <pre className="min-h-[280px] overflow-auto rounded-md bg-secondary p-4 text-sm">
              {response ? formatJson(response) : "等待发送 action..."}
            </pre>
          </section>

          <section className="rounded-xl border bg-card p-4 text-card-foreground shadow-sm">
            <h2 className="mb-3 text-lg font-semibold">Events</h2>
            {response?.events.length ? (
              <ol className="space-y-3">
                {response.events.map((event) => (
                  <li
                    key={event.seq}
                    className="rounded-md border bg-background p-3 text-sm"
                  >
                    <div className="font-medium">
                      #{event.seq} {event.type}
                    </div>
                    <pre className="mt-2 overflow-auto text-xs text-muted-foreground">
                      {formatJson(event)}
                    </pre>
                  </li>
                ))}
              </ol>
            ) : (
              <p className="text-sm text-muted-foreground">暂无事件</p>
            )}
          </section>

          <section className="rounded-xl border bg-card p-4 text-card-foreground shadow-sm">
            <h2 className="mb-3 text-lg font-semibold">Error</h2>
            <pre className="overflow-auto rounded-md bg-secondary p-4 text-sm">
              {response?.error ? formatJson(response.error) : "无错误"}
            </pre>
          </section>
        </div>
      </section>
    </main>
  );
}
