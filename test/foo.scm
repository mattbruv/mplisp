
; this is a comment test
(define (take-while p xs)
  (if (or (null? xs)
          (not (p (car xs))))
      '()
      (cons (car xs) (take-while p (cdr xs)))))

(define (collect str delim acc)
  (let ((line (take-while (lambda (x) (not (eq? x delim))) str)))
    (if (null? str)
        acc
        line)))

(define (string-split str delim)
  (collect (string->list str) #\newline '()))

(define file (call-with-input-file "C:\\Users\\MattP\\Documents\\AOC-2021\\src\\day03\\input.txt" get-string-all))

(define input (string-split file "\n"))

(display input)