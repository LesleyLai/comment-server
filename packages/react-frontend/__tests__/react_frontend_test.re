open Jest;

describe("1 + 1 == 2", () => {
  open Expect;
  open! Expect.Operators;

  test("1+1", () =>
       expect(1 + 1) == 2);
});
