use texture_packer::funcs::util::find_name;

#[test]
fn test_find_name() {
  assert_eq!(find_name(""), "");
  assert_eq!(find_name("a"), "a");
  assert_eq!(find_name("b"), "b");
  assert_eq!(find_name("c"), "c");
  assert_eq!(find_name("ca"), "ca");
  assert_eq!(find_name("cb"), "cb");
  assert_eq!(find_name("db"), "db");
  assert_eq!(find_name("dba"), "dba");
  assert_eq!(find_name("dba/"), "");
  assert_eq!(find_name("dba/a"), "a");
  assert_eq!(find_name("dba\\a"), "a");
  assert_eq!(find_name("dba\\a."), "a");
  assert_eq!(find_name("dba\\a.b"), "a");
}
