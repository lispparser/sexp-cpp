
class Lisp
{
const Lisp*
Lisp::get_lisp(const char* name) const
{
  for(const Lisp* p = this; p != 0; p = p->get_cdr()) {
    const Lisp* child = p->get_car();
    if(!child || child->get_type() != TYPE_CONS)
      continue;
    const Lisp* childname = child->get_car();
    if(!childname)
      continue;
    std::string childName;
    if(!childname->get(childName))
      continue;
    if(childName == name) {
      return child->get_cdr();
    }
  }

  return 0;
}

  /** convenience functions which traverse the list until a child with a
   * specified name is found. The value part is then interpreted in a specific
   * way. The functions return true, if a child was found and could be
   * interpreted correctly, otherwise false is returned and the variable value
   * is not changed.
   * (Please note that searching the lisp structure is O(n) so these functions
   *  are not a good idea for performance critical areas)
   */
  template<class T>
  bool get(const char* name, T& val) const
  {
    const Lisp* lisp = get_lisp(name);
    if(!lisp)
      return false;

    if(lisp->get_type() != TYPE_CONS)
      return false;
    lisp = lisp->get_car();
    if(!lisp)
      return false;
    return lisp->get(val);
  }

  template<class T>
  bool get(const char* name, std::vector<T>& vec) const
  {
    vec.clear();

    const Lisp* child = get_lisp(name);
    if(!child)
      return false;

    for( ; child != 0; child = child->get_cdr()) {
      T val;
      if(!child->get_car())
        continue;
      if(child->get_car()->get(val)) {
        vec.push_back(val);
      }
    }

    return true;
  }

  const Lisp* get_lisp(const char* name) const;
  const Lisp* get_lisp(const std::string& name) const
  { return get_lisp(name.c_str()); }



void
Lisp::print(int indent) const
{
  for(int i = 0; i < indent; ++i)
    printf(" ");

  if(type == TYPE_CONS) {
    printf("(\n");
    const Lisp* lisp = this;
    while(lisp) {
      if(lisp->v.cons.car)
        lisp->v.cons.car->print(indent + 1);
      lisp = lisp->v.cons.cdr;
    }
    for(int i = 0; i < indent; ++i)
      printf(" ");
    printf(")");
  }
  if(type == TYPE_STRING) {
    printf("'%s' ", v.string);
  }
  if(type == TYPE_INTEGER) {
    printf("%d", v.integer);
  }
  if(type == TYPE_REAL) {
    printf("%f", v.real);
  }
  if(type == TYPE_SYMBOL) {
    printf("%s ", v.string);
  }
  if(type == TYPE_BOOLEAN) {
    printf("%s ", v.boolean ? "true" : "false");
  }
  printf("\n");
}

};
