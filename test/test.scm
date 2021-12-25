
;(car '(1 2 3 4))

;(empty? (quote ()))
(define gt5 (lambda (x) (> x 5)))
(filter gt5 '(1 2 3 4 5 6 7 8 9 10))

;(quote (1 2 3 4))