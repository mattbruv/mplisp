; takes two expressions and evaluates them in order

(define and (lambda (a b) (if a (if b true false) false)))
(define or (lambda (a b) (if a true (if b true false))))
