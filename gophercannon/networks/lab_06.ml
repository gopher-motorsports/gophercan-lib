let rev lst = List.fold_left (fun a x -> x::a) [] lst

let partition_compare (f: 'a -> int) (lst: 'a list) : 'a list * 'a list * ' list =
  let choose (x: 'a) ( buckets : ('a list * 'a list * 'a list)) : 
        ('a list * 'a list * 'a list) = 
    match buckets with
    | (yess, nos) -> 
       if f x 
       then (x :: yess, nos)
       else (yess, x :: nos)
  in
  List.fold_right choose lst ([], [], [])