(define xMin 20)
(define xMax 30)
(define yMin -10)
(define yMax -5)

(define inBounds (lambda (x y)
    (and (inX x) (inY y))))

(define inX (lambda (x) 
    (and (>= x xMin) (and (<= x xMax) true))))

(define inY (lambda (y) 
    (and (>= y yMin) (and (<= y yMax) true))))

(define pastX (lambda (x) (> x xMax)))
(define pastY (lambda (y) (< y yMin)))
(define pastBounds (lambda (x y) (or (pastX x) (pastY y))))

(define step (lambda (x y xVel yVel peak)
    ; if we have hit our magic area, return highest peak
    (if (inBounds x y) peak
        ; if we are past the bounds, return FAIL
        (if (pastBounds x y) false
            ; otherwise, do the next step
            (step
                (+ x xVel)
                (+ y yVel)
                (if (> xVel 0) (- xVel 1)
                    (if (< xVel 0) (+ xVel 1) 0))
                (- yVel 1)
                (if (> y peak) y peak))))))

(define shoot (lambda (xVel yVel) (step 0 0 xVel yVel 0)))

(shoot 6 9)


;(define range (lambda (from to acc) (if (= from (+ 1 to)) acc
;    (range (+ from 1) to (cons (- to from) acc)))))

;(range 0 500 '())