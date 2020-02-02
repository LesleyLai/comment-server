open Jest;

describe("1 + 1 == 2", () => {
  open Expect;
  open! Expect.Operators;

  open Server;
  module MyServer = MakeServer(MockDatabase);
  let app = MyServer.create;
});
