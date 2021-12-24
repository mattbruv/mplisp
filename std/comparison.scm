
(define < (lambda (a b) (> b a)))

(define = (lambda (a b) (if (> a b) false 
    (if (< a b) false true))))

(define >= (lambda (a b) (if (> a b) true
    (if (= a b) true false))))

(define <= (lambda (a b) (>= b a)))