FROM node:alpine
WORKDIR /app
COPY ./app/package.json ./app/package-lock.json ./
RUN apk add --no-cache python3 make g++ && npm ci --omit=dev
CMD ["npm", "run", "dev"]
