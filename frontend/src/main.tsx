import React from "react";
import ReactDOM from "react-dom/client";
import { DebugAction } from "@/pages/DebugAction";
import "./styles.css";

ReactDOM.createRoot(document.getElementById("root")!).render(
  <React.StrictMode>
    <DebugAction />
  </React.StrictMode>,
);
