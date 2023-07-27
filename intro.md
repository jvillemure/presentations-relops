# Purpose of this talk..

This talk fits in a wider scheme of a general coding idiom that is called 'Value Semantics'.
Here we explore a subfield of Value Sematics that is concerned with the expression and calculation
of what we call object Equivalence and Order.
explores

# Relational operations

These are binary predicates on two objects, that model specifc semantics (we'll see soon...);
in other words they are pure functions that takes two objects of a single types (usually) and return a boolean.

There two fundamental categories of  relational operations;

- Equality: computes object equivalence;
  In C++: operator==

- Ordering: computes the relative ranking of objects
  In C++: operator<

There is also a common operation in programming languages which combines
both equality and order as a single operation which is commonly known as:

- Three way comparison: instead of returning a bool,
  it returns one of three values 'smaller', 'equal' or 'larger' (or -1, 0, +1 in some languages).
  In C++ (since C++20): operator<=>

## Why they are useful...

Those are actually drawn from the world of maths,
and carry on all the potential use cases that they would have outside the domain of computer science.

In the computer science domain, we build on top their mathematical properties to
make it possible to design the fastest possible data structures and algorithms:

- Data structures: associative containers (both ordered and hashed), ordered trees, heaps,
  priority queues, etc. We often overlook the opportunity to make good use of specially organized
  data structure to solve a computational problem...

- Algorithms: binary search, sorting, partitioning, finding medians, etc.

## Fundamental properties

Apart from the name (signature) of the relational operator, what makes the '=='/'<' an
Equality/Ordering relationship?

Actually, the name/signature is not a warrant that an operation is an Equality or Ordering relationship.
What makes an operation an Equality (Ordering) is only determined by the semantics modeled by the
function's implementation. To qualify as an Equality (Ordering), it is imperative and sufficient 
('if and only if'; 'iff' for short) that the function's implementation comply with the mathematical
definition of 'Equality' ('Ordering'), plus few other computer science related properties (that have
no equivalent in the pure math domain). These semantic properties are universal in the domain of
computer science, they apply to any programming language.

### The definition of Equality

The implementation of an equality operation, must model the mathematics definition of Equivalence.
A relation '==' is an Equivalence relationhsip iff for any three objects 'x1', 'x2', 'x3',
the following relationships hold:

1) Reflexivity: 'x1 == x1'
   In other words, any object is always equal to itself

2) Symmetry: if 'x1 == x2' then 'x2 == x1'
   In other words, if an object 'this' compares equal to an object 'that',
   then 'that' also compares equal to 'this'.

3) Transitivity: if 'x1 == x2' and 'x2 == x3' then 'x1 == x3'
   In other words, if an object 'this' compares equal to both objects 'that1' and 'that2'
   then 'that1' and 'that2' also compare equal. Performance of algo/data structures rely
   heavily on this property.
  
Normally, reflexivity and symmetry are easily modeled, but unfortunately transitivity is often overlooked.
However, failure to meet those three requirements lead to unexpected, even erratic behavior for
algorithms and data structures.

### The definition of Ordering

The implementation of an Ordering operation, must model the mathematics definition of Ordering.
A relation '<' is an Ordering relationhsip iff for any three objects 'x1', 'x2', 'x3',
the following relationships hold:

1) Antireflexivity: '!(x1 < x1)'
   In other words, any object is never smaller than itself

2) Asymmetry: if 'x1 < x2' then '!(x2 < x1)'
   In other words, if an object 'this' compares smaller to an object 'that',
   then 'that' do not compare smaller to 'this'.

3) Transitivity: if 'x1 < x2' and 'x2 < x3' then 'x1 < x3'
   In other words, if an object 'this' compares smaller than 'that1', and 'that1' compares smaller
   than 'that2', then 'this' compares smaller that 'that2'.
   Performance of algo/data structures rely heavily on this property.
  
Normally, Antireflexivity and Asymmetry are easily modeled, but unfortunately Transitivity is often overlooked.
However, failure to meet those three requirements lead to unexpected, even erratic behavior for
algorithms and data structures.

### Semantic constraints of programming

Furthermore, in the domain of programming, there are other few additional semantic requirements that
needs to be met for both the Equality and Ordering:

4) They must not produce _visible_ side effects. Only a value computation that yields a bool.
   Modifying mutable state of arguments is okay only if that mutable state is properly used
   to implement 'semantic invariant' state for the purpose of gaining performance.
   That is state which do not affect any observable behavior (ie, caching, flyweight patterns)

The following statements describes the notion of Structural Equality:
5) They must only use the two object arguments, no hidden operands. In other words, the
outside world must not interact with their implementation.


6) Only non mutable sate should be used to carry the computation. In other words, caching must
not affect the outcome. 

7.a) If implementing a class's natural Equality (or Ordering), (ie the friend operator==, or friend operator<)
     then all non mutable state must be taken into account in the implementation.

7.b) Custom function objects implementing an Equality (or Ordering) on a type are not required to implement
a full memberwise comparison (ie they may skip some members).
This is usually the intent of permitting custom Equality for a specific use of an algorithm or data structure.
For instance comparing case insensitive equality,
or other Equality that would be coarsere that the class's default).

The point 7.a) give rise to two important consequences:
 - if 'x1 == x2', then we could use 'x2' in place of 'x1' anywhere in code and observe the same behavior.
 - Two incomparable objects (with resp. to Ordering) must be equal:
   That is: if '!(x1 < x2) && !(x2 < x1)' then 'x1 == x2'
   (the converse is already guaranteed by deduction from the fundamental properties)

The operations related to Copying or Moving and Hash calculations, must also be taken into account
when we define Equality or Ordering.

8) A copy creates an equal object, in other words, the copy does a deep copy (as far as the
   primary semantics of the object's is concerned)
8.1) In c++, idem for a move operation

9) The hash value of two equal objects must be equal.

## Properties of relationships

In mathematical terminology we say that a relationship 'R' is finer than relationship 'S'
iff for any objects 'x' and 'y'

- if 'x R y' then 'x S y'

We also say that a relationship 'S' is coarser than 'R'.

In the context of an Equivalence, a relationship 'R' is finer than 'S' if 'S'
can differentiate two objects that 'R' would consider equivalent.

Example: lexicographic comparison of strings of chars is finer than case insensitive
lexicographic comparison of strings of chars.

## Classification of Equivalences

### Identity

This is the finest equivalence relationship: two objects are identical if they are the same objects.
In the programming world, we identity is defined as (in C++):

```cpp
bool ident(T const& lhs, T const& rhs )
{
   return &lhs == &rhs;
}
```

This relationship is not as useful as one might think, and is certainly not a good candidate
for implementing "operator==": two different objects with the same value must compare equal.
This is necessary for implementing the notion of Value Semantics.

### Value Equivalence

Two objects 'x' and 'y' are value equivalent any operation 'f' that uses 'x'
could be rewritten using the object 'y' and:

- 'f(x)' == 'f(y)'
- both 'f(x)' and 'f(y)' would produce the same observable side effects

Unless 'f' 's behavior would depend on the object's identity.

Value equivalence is exactly what the `operator==` should implement.


## Classification of Orderings

Ordering relationships have several sub categories...

### Strong ordering

This is the usual ordering of numbers for instance. We should aim to define
Ordering and equality to be of this kind

- For any object 'x', 'y' either 'x < y', 'y < x', or 'x == y'

In other words, all objects are comparable.

### Strict Weak Ordering

This is normally a strict requirement for working with data structures and algorithms,
it is similar to strong ordering, in that incomparable object are equivalent. This
is the kind of ordering that results from comparing only part of a classe's members, or by truncating
the full value before using the usual <. (A strong ordering is a strict weak ordering).

- Incomparability of objects is an equivalence relationship:
  that is: !(x < y) && !(y < x) is an Equivalence.

In other words, a Strict weak ordering may admit incomparable distinct objects;
but 
but any two incomparable objects are

Example1: lexicographic ordering of strings of chars is finer than case insensitive
lexicographic ordering of strings of chars.

Example2:


### Partial Ordering

For the purpose of programming, any Ordering which is not a Strict Weak Ordering is considered a
partial ordering. This is not good enough for algos and data structures. 
