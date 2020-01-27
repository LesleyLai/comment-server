# Comments Server

[![Build Status](https://travis-ci.org/LesleyLai/comment-server.svg?branch=master)](https://travis-ci.org/LesleyLai/comment-server)

I am tired of third-party comment servers, so I try to build one to power [my blog](https://lesleylai.info). It is not in a usable stage yet.

## Develop
### Bootstrap
```
lerna bootstrap
```

### Build
```
yarn build
```

### Test

```
yarn test
```

Need to `build` first.

### Watch

Unfortunately, I cannot find a way to make lerna work with bucklescript compiler yet.
Currently, you can `cd` into folders of each individual packages and watch build on them with `yarn watch`.

In another terminal

```
yarn watch-run
```

to watch change in the source and automatically restart the server.
