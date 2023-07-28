; xshevc01
; run with
;
;   $ z3 -smt2 queens.smt
 
; declare the sorts of pos --- pos represents the chessboard (an 8x8 array) ,
; indexed by "(pos row column)":
; true  --- a queen is on [row, column]
; false --- a queen is not on [row, column]
(declare-fun pos (Int Int) Bool)
 
; a helper function that checks that 1 <= x <= 8
(define-fun is-in-range ((x Int)) Bool (and (> x 0) (<= x 8)))
 
; first, we say that in all rows, there is at least one queen 
(assert (forall ((i Int)) (=> (is-in-range i) (exists ((j Int)) (and (is-in-range j) (pos i j)) ))))
; second, we say that if a queen is on [k; l], then there is no queen on any [m; l] (for m != k) and on any [k; m] (for m != l)
(assert (forall ((k Int)) (forall ((l Int)) (=> (is-in-range k) (=> (pos k l) (forall ((m Int)) (=> (is-in-range m) (and (=> (not (= m k)) (not (pos m l))) (=> (not (= m l)) (not (pos k m))))))) ))))
 
; ADD YOUR CONSTRAINTS HERE
;============================= START ==============
; third, we say that if a queen is on [k; l], then there is no queen on any [k+m; l+m], [k+m; l-m], [k-m; l+m], [k-m; l-m] for any 1<=m<=8 within our field
; this is a working formula, but too hard for SMT solver:
; (assert (forall ((k Int)) (forall ((l Int)) (=> (is-in-range k) (=> (pos k l) (forall ((m Int)) (=> (is-in-range m) (and (=> (is-in-range (+ k m)) (and (=> (is-in-range (+ l m)) (not (pos (+ k m) (+ l m)))) (=> (is-in-range (- l m)) (not (pos (+ k m) (- l m)))))) (=> (is-in-range (- k m)) (and (=> (is-in-range (+ l m)) (not (pos (- k m) (+ l m)))) (=> (is-in-range (- l m)) (not (pos (- k m) (- l m))))))))))))))


; there is max 1 queen on the main diagonal
(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k k) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m m)))))))))
 
; there is max 1 queen on the secondary diagonal
(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 9 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 9 m))))))))))

; max 1 queen on other giadonals

; ⟍ ⟍ ⟍ ⟍ ⟍ ⟍ ⟍ ↘
; ⟍ ⟍ ⟍ ⟍ ⟍ ⟍ ⟍ ↘
; ⟍ ⟍ ⟍ ⟍ ⟍ ⟍ ⟍ ↘
; ⟍ ⟍ ⟍ ⟍ ⟍ ⟍ ⟍ ↘
; ⟍ ⟍ ⟍ ⟍ ⟍ ⟍ ⟍ ↘
; ⟍ ⟍ ⟍ ⟍ ⟍ ⟍ ⟍ ↘
; ⟍ ⟍ ⟍ ⟍ ⟍ ⟍ ⟍ ↘
; ↘ ↘ ↘ ↘  ↘ ↘ ↘ ⟍
(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos (+ 1 k) k) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos (+ 1 m) m)))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos (+ 2 k) k) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos (+ 2 m) m)))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos (+ 3 k) k) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos (+ 3 m) m)))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos (+ 4 k) k) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos (+ 4 m) m)))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos (+ 5 k) k) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos (+ 5 m) m)))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos (+ 6 k) k) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos (+ 6 m) m)))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos (+ 7 k) k) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos (+ 7 m) m)))))))))


(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (+ 1 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (+ 1 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (+ 2 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (+ 2 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (+ 3 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (+ 3 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (+ 4 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (+ 4 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (+ 5 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (+ 5 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (+ 6 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (+ 6 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (+ 7 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (+ 7 m))))))))))




; ↙ ⟋ ⟋ ⟋ ⟋ ⟋ ⟋ ⟋
; ↙ ⟋ ⟋ ⟋ ⟋ ⟋ ⟋ ⟋ 
; ↙ ⟋ ⟋ ⟋ ⟋ ⟋ ⟋ ⟋ 
; ↙ ⟋ ⟋ ⟋ ⟋ ⟋ ⟋ ⟋ 
; ↙ ⟋ ⟋ ⟋ ⟋ ⟋ ⟋ ⟋ 
; ↙ ⟋ ⟋ ⟋ ⟋ ⟋ ⟋ ⟋ 
; ↙ ⟋ ⟋ ⟋ ⟋ ⟋ ⟋ ⟋ 
; ⟋ ↙ ↙ ↙  ↙ ↙ ↙ ↙ 
(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 8 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 8 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 7 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 7 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 6 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 6 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 5 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 5 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 4 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 4 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 3 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 3 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 2 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 2 m))))))))))


(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 10 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 10 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 11 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 11 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 12 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 12 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 13 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 13 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 14 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 14 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 15 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 15 m))))))))))

(assert (forall ((k Int)) (=> (is-in-range k) (=> (pos k (- 16 k)) (forall ((m Int)) (=> (is-in-range m) (=> (not (= m k)) (not (pos m (- 16 m))))))))))
;============================= END ================
 
(declare-const y-pos-a Int)
(declare-const y-pos-b Int)
(declare-const y-pos-c Int)
(declare-const y-pos-d Int)
(declare-const y-pos-e Int)
(declare-const y-pos-f Int)
(declare-const y-pos-g Int)
(declare-const y-pos-h Int)
 
(assert (and (is-in-range y-pos-a) (pos 1 y-pos-a)))
(assert (and (is-in-range y-pos-b) (pos 2 y-pos-b)))
(assert (and (is-in-range y-pos-c) (pos 3 y-pos-c)))
(assert (and (is-in-range y-pos-d) (pos 4 y-pos-d)))
(assert (and (is-in-range y-pos-e) (pos 5 y-pos-e)))
(assert (and (is-in-range y-pos-f) (pos 6 y-pos-f)))
(assert (and (is-in-range y-pos-g) (pos 7 y-pos-g)))
(assert (and (is-in-range y-pos-h) (pos 8 y-pos-h)))
 
(check-sat)
(get-model)