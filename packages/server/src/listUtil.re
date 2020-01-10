let addIf = (lst, condition, toAppend) => {
  if (condition) {
    [toAppend, ...lst]
  } else {
    lst
  }
};
