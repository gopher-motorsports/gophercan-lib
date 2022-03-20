
let eqf (f1: float) (f2: float) : bool = 
  (* We will not check floating point values for exact
     equality since rounding errors can derail this method.
     Instead we determine that the values are "close enough"
     together by checking that the absolute value of their 
     difference is within a specified value, `epsilon`.
   *)
  let epsilon = 0.01
  in Float.abs (f1 -. f2) < epsilon

let rec eqfs (floats1: float list) (floats2: float list) : bool =
  match floats1, floats2 with
  | [], [] -> true
  | [], _  -> false
  | _,  [] -> false
  | f1::fs1, f2::fs2 -> eqf f1 f2 && eqfs fs1 fs2
 
type test = unit -> unit

let eval_test (f: unit -> ('a * 'a)) (expr_str: string)
      (show: 'a -> string) : test =
  fun () ->
  (try
     let expr, expected = f ()
     in
     if expr = expected then
       let msg = "PASSED:\n    `" ^ expr_str ^ 
                   "`\n correctly evaluates to\n    `" ^
                     show expected ^ "`"
       in
       print_endline msg
     else
       let msg = "FAILED:\n    `" ^ expr_str ^ 
                   "`\n  inccorrectly evaluates to\n    `" ^
                     show expr ^ "`\n  but should have evaluated to\n    `" ^
                     show expected ^ "`"
       in
       print_endline msg
   with
   | _ ->
      let msg = "FAILED:\n    `" ^ expr_str ^ 
                   "`\n  raised an exception but should have computed a value."
       in
       print_endline msg
  )

let show_list (show_elem: 'a -> string) (lst: 'a list) : string =
  (* A function to convert lists to strings. *)
  "[" ^ (String.concat "; " (List.map show_elem lst)) ^ "]"

let show_pair (show_fst: 'a -> string) (show_snd: 'b -> string)
      ((a, b) : 'a * 'b) : string =
  "(" ^ show_fst a ^ ", " ^ show_snd b ^ ")"

let show_triple (show_fst: 'a -> string) (show_snd: 'b -> string) (show_trd: 'c -> string)
      ((a, b, c) : 'a * 'b * 'c) : string =
  "(" ^ show_fst a ^ ", " ^ show_snd b ^ ", " ^ show_trd c ^ ")"

let show_string (s: string) : string =
  "\"" ^ s ^ "\""