import eslintReact from "@eslint-react/eslint-plugin";
import css from "@eslint/css";
import eslintJs from "@eslint/js";
import markdown from "@eslint/markdown";
import eslintConfigPrettier from "eslint-config-prettier/flat";
import { defineConfig } from "eslint/config";
import globals from "globals";

export default defineConfig([
    {
        files: ["**/*.js", "**/*.jsx"],
        extends: [eslintJs.configs.recommended, eslintReact.configs.recommended],
        languageOptions: {
            globals: {
                ...globals.browser,
                process: "readonly",
            },
            parserOptions: {
                ecmaFeatures: {
                    jsx: true,
                },
            },
        },

        // Custom rule overrides
        rules: {
            "@eslint-react/no-missing-key": "warn",
            "no-unused-vars": "warn",
        },
    },
    {
        files: ["**/*.md"],
        plugins: { markdown },
        language: "markdown/gfm",
        extends: ["markdown/recommended"],
    },
    {
        files: ["**/*.css"],
        plugins: { css },
        language: "css/css",
        extends: ["css/recommended"],
    },
    eslintConfigPrettier,
]);
