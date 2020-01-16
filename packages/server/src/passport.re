module Passport = {
  type strategy;

  [@bs.module "passport"] [@bs.val] external use: strategy => unit = "use";

  module GoogleStrategy = {
    [@bs.module "passport-google-oauth20"] [@bs.new]
    external create: unit => strategy = "Strategy";
  };
};

// Passport.use(Passport.GoogleStrategy.create());
