import { defineConfig } from "vite";

import { viteSingleFile } from "vite-plugin-singlefile";

export default defineConfig({
  build: {
    minify: "terser",
    sourcemap: false,
    rollupOptions: {
      plugins: [viteSingleFile()],
    },
  },
  plugins: [
    {
      name: "minify-html",
      transformIndexHtml(html) {
        return html
          .replace(/<!--[\s\S]*?-->/g, "")
          .replace(/[\n\r\t]/g, "")
          .replace(/>\s+</g, "><")
          .replace(/\s{2,}/g, " ")
          .trim();
      },
    },
  ],
});
