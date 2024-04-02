# Learning Isabelle

---
_11.03.2024_

https://inria.hal.science/hal-01208577/document

Example theory for isabelle

```
theory Scratch
  imports Main
begin
fun remove :: "'a ⇒ 'a list ⇒ 'a list"
  where
    "remove x [] = []" |
    "remove x (y#ys) = (if x=y then (remove x ys) else y#(remove x ys))"

value "remove 2 [1,2,3]::(int list)"

lemma "¬ (List.member (remove e l) e)"
  nitpick
  sorry
lemma "(length l) = (length (remove e l)) + (count_list l e)"
  nitpick
  sorry
end
```

We have to use `(2::int)` because otherwise it can not solve the types.

Alternatively we can do:

```
value "remove int [1::int,2,3]"
value "remove 2 [1,2,3]::(int list)"
value "(remove 2 [1,2,3])::(int list)"
```

__nitpick__ command tries to find the counterexample

__sorry__ means that the proof is not finished

If we apply following lemma:
```
lemma "(length l) = (length (remove e l)) + (count l e)"
```

We have the result:
```
Auto Quickcheck found a counterexample:
  l = [a1]
  e = a1
  count = λx xa. 0
Evaluated terms:
  length l = 1
  length (remove e l) + count l e = 0
```

Here, it found both l, e and count, because count is not a known name (we should use `count_list instead`)	

### Proof example
```
lemma "(length l) = (length (remove e l)) + (count_list l e)"
  apply (induct l)
   apply auto
  done
```

and from this in output window we have
```
goal (2 subgoals):
 1. length [] = length (remove e []) + count_list [] e
 2. ⋀a l. length l = length (remove e l) + count_list l e ===> length (a # l) = length (remove e (a # l)) + count_list (a # l) e
```

### What is apply and by?
```
lemma "(length (remove e l)) = (length l) ⟶ ¬(List.member l e)"
  apply (induct l)
   apply auto
     apply (simp add: member_rec(2))
  apply (metis count_remove impossible_Cons le_add1 length_Cons)
  apply (metis count_remove impossible_Cons le_add1 length_Cons)
  by (simp add: member_rec(1))
end
```

---
_13.03.2024_

https://isabelle.in.tum.de/doc/prog-prove.pdf

We write __[simp]__ to show that lemma can be used for simplification

---
_15.03.24_

### What is metis?

---
_22.03.24_

### How to split varaibles?

TODO:
* simplfy proof 2 vectors
* write explanation

## Butterfly network

https://slideplayer.com/slide/14466155/

https://eng.libretexts.org/Bookshelves/Computer_Science/Programming_and_Computation_Fundamentals/Mathematics_for_Computer_Science_(Lehman_Leighton_and_Meyer)/02%3A_Structures/10%3A_Communication_Networks/10.08%3A_Butterfly


---
_29.03.24_

Graph theory for Isabelle

https://www.isa-afp.org/browser_info/current/AFP/Graph_Theory/outline.pdf