# Frontend

React + Vite Debug Console for the Abel Tomato Soul JSON Contract.

## UI Stack

This frontend uses:

```txt
React
Vite
TypeScript
Tailwind CSS
shadcn/ui-style components
```

shadcn/ui conventions are configured by `components.json`:

```txt
components -> src/components
ui         -> src/components/ui
utils      -> src/lib/utils.ts
```

Current base component:

```txt
src/components/ui/button.tsx
```

## Debug Console

Current page:

```txt
src/pages/DebugAction.tsx
```

The page supports:

```txt
1. Load example request from GET /api/debug/example-action.
2. Edit Request JSON in a textarea.
3. Send POST /api/game/action.
4. Display full Response JSON.
5. Display events list and error block.
```

## Setup

```bash
cd frontend
pnpm install
```

## Run

```bash
pnpm dev
```

## Validate

```bash
pnpm typecheck
```

## Backend

During development, Vite proxies `/api` to `http://127.0.0.1:8000`.