FROM node:26-alpine AS development-dependencies-env
COPY frontend/package.json /app/frontend/
COPY package.json package-lock.json /app/
WORKDIR /app/
RUN npm ci

FROM node:26-alpine AS production-dependencies-env
COPY frontend/package.json /app/frontend/
COPY package.json package-lock.json /app/
WORKDIR /app/
RUN npm ci --omit=dev

FROM node:26-alpine AS build-env
COPY --from=development-dependencies-env /app/node_modules /app/node_modules
COPY frontend /app/frontend
WORKDIR /app/frontend
RUN npm run build

FROM node:26-alpine AS serve-env
COPY --from=production-dependencies-env /app /app/
COPY --from=build-env /app/frontend/server.js /app/frontend/package.json /app/frontend/
COPY --from=build-env /app/frontend/build /app/frontend/build
WORKDIR /app/frontend
CMD ["npm", "run", "start"]