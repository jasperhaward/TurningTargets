import { render } from "preact";
import App from "./App";
import "./index.scss";

if (import.meta.env.MODE === "development") {
  import("preact/debug");
}

render(<App />, document.body);
