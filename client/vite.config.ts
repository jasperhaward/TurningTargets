import { IncomingMessage, ServerResponse } from "http";
import { defineConfig } from "vite";
import preact from "@preact/preset-vite";
import config from "./config.json";

function configBypass(
  req: IncomingMessage,
  res: ServerResponse | undefined
): false {
  if (!res) {
    return false;
  }

  if (req.method === "POST") {
    // update config
  } else {
    res.setHeader("Content-Type", "application/json");
    res.end(JSON.stringify(config));
  }

  return false;
}

export default defineConfig({
  build: {
    outDir: "build",
    rollupOptions: {
      output: {
        entryFileNames: "[name].js",
        chunkFileNames: "[name].js",
        assetFileNames: "[name].[ext]",
      },
    },
  },
  plugins: [preact()],
  server: {
    proxy: {
      "/config": {
        bypass: configBypass,
      },
    },
  },
});
