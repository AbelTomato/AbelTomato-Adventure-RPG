import React from "react";
import ReactDOM from "react-dom/client";
import { ConfigViewer } from "@/pages/ConfigViewer";
import { DebugAction } from "@/pages/DebugAction";
import { SaveList } from "@/pages/SaveList";
import "./styles.css";

type ActivePage = "configs" | "debug" | "saves";

const navItems: { id: ActivePage; label: string }[] = [
  { id: "saves", label: "Saves" },
  { id: "configs", label: "Configs" },
  { id: "debug", label: "Debug" },
];

function App() {
  const [activePage, setActivePage] = React.useState<ActivePage>("saves");

  const activeContent = {
    configs: <ConfigViewer />,
    debug: <DebugAction />,
    saves: <SaveList />,
  }[activePage];

  return (
    <>
      <nav className="border-b bg-card px-6 py-3">
        <div className="mx-auto flex max-w-7xl gap-2">
          {navItems.map((item) => (
            <button
              key={item.id}
              className={`rounded-md px-3 py-2 text-sm font-medium ${
                activePage === item.id
                  ? "bg-primary text-primary-foreground"
                  : "text-muted-foreground hover:bg-accent hover:text-accent-foreground"
              }`}
              type="button"
              onClick={() => setActivePage(item.id)}
            >
              {item.label}
            </button>
          ))}
        </div>
      </nav>
      {activeContent}
    </>
  );
}

ReactDOM.createRoot(document.getElementById("root")!).render(
  <React.StrictMode>
    <App />
  </React.StrictMode>,
);
