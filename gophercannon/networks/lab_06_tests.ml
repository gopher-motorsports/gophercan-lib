(* You are welcome to read this file, but you do not need
   to understand (yet) its contents.  We will discuss 
   several aspects of this later in the semester. For now
   you only need to use it, not understand it.
 *)

open Lab_06
open Tests

let compare_to_3 a = compare a 3
let comp_3_str a = compare (String.length a) 3 
let comp_polarity a = if a mod 2 = 0 then 1 else -1
let comp_3_mod a = if a mod 3 = 0 then -1 else if a mod 3 = 1 then 0 else 1

let some_tests =
  [
    eval_test (fun () -> 
        rev [1;2;3;4;5], [5;4;3;2;1])
        "rev [1;2;3;4;5]"
        (show_list string_of_int);

    eval_test (fun () ->
        rev ["hello"; "world"], ["world"; "hello"])
        "rev [\"hello\";\"world\"]"
        (show_list show_string);

    eval_test (fun () ->
        rev [("hi", 1); ("bye", 3); ("hello", 1)], 
        [("hello", 1); ("bye", 3); ("hi", 1)])
        "rev [(\"hi\", 1); (\"bye\", 3); (\"hello\", 1)]"
        (show_list (show_pair show_string string_of_int));

    eval_test (fun() ->
        rev [], [])
        "rev []"
        (show_list show_string);

    eval_test (fun () ->
        partition_compare compare_to_3 [1;2;3;4;5;6;7],
        ([1;2], [3], [4;5;6;7]))
        "partition_compare [1;2;3;4;5;6;7]"
        (show_triple (show_list string_of_int) (show_list string_of_int) (show_list string_of_int));

    eval_test (fun () ->
        partition_compare comp_3_str ["a";"ad";"bee";"tree";"see";"zealotry"],
        (["a";"ad"], ["bee"; "see"], ["tree"; "zealotry"]))
        "partition_compare comp_3_str [\"a\";\"ad\";\"bee\";\"tree\";\"see\";\"zealotry\"]"
        (show_triple (show_list show_string) (show_list show_string) (show_list show_string));

    eval_test (fun () ->
        partition_compare comp_polarity [1; 2; 3; 4],
        ([1; 3], [], [2; 4]))
        "partition_compare comp_polarity [1; 2; 3; 4]"
        (show_triple (show_list string_of_int) (show_list string_of_int) (show_list string_of_int));

    eval_test (fun () ->
        partition_compare comp_3_mod [0;1;2;3;4;5;6],
        ([0;3; 6], [1;4],[2;5]))
        "partition_compare comp_3_mod [0;1;2;3;4;5;6]"
        (show_triple (show_list string_of_int) (show_list string_of_int) (show_list string_of_int));
        
    eval_test (fun () ->
        create_new_if_third_odd [1;2;3;4;5;6;7;8;9;10;11;12],
        [[1;2];[3;4;5;6;7;8];[9;10;11;12]])
        "create_new_if_third_odd [1;2;3;4;5;6;7;8;9;10;11;12]"
        (show_list (show_list string_of_int));

    eval_test (fun () ->
        create_new_if_third_odd [2;4;6;8;10;12],
        [[2;4;6;8;10;12]])
        "create_new_if_third_odd [2;4;6;8;10;12]"
        (show_list (show_list string_of_int));

    eval_test (fun () ->
        create_new_if_third_odd [1;3;3;5;5;7;9],
        [[1;3];[3;5;5];[7;9]])
        "create_new_if_third_odd [1;3;3;5;5;7;9]"
        (show_list (show_list string_of_int));

    eval_test (fun () ->
        create_new_if_third_odd [], [])
        "create_new_if_third_odd []"
        (show_list (show_list string_of_int));

    eval_test (fun () ->
        fold_map (fun a -> a + 3) [1;2;3], [4;5;6])
        "fold_map (fun a -> a + 3) [1;2;3]"
        (show_list string_of_int);

    eval_test (fun () ->
        fold_map (fun b -> b ^ "!") ["hello"; "world"], ["hello!"; "world!"])
        "fold_map (fun b -> b ^ \"!\") [\"hello\"; \"world\"]"
        (show_list show_string);

    eval_test (fun () ->
        fold_map (fun a -> a * 2) [1;2;3], [2;4;6])
        "fold_map (fun a -> a * 2) [1;2;3]"
        (show_list string_of_int);

    eval_test (fun () ->
        fold_map (fun a -> a) [], [])
        "fold_map (fun a -> a) []"
        (show_list string_of_int);
  ]

let run_tests tests = List.iter (fun f -> f ()) tests

let _ = run_tests some_tests