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

(define step (lambda (x y xVel yVel peak steps)
    ; if we have hit our magic area, return highest peak
    (if (inBounds x y) (cons peak (cons x (cons y (cons steps '()))))
        ; if we are past the bounds, return FAIL
        (if (pastBounds x y) false
            ; otherwise, do the next step
            (step
                (+ x xVel)
                (+ y yVel)
                (if (> xVel 0) (- xVel 1)
                    (if (< xVel 0) (+ xVel 1) 0))
                (- yVel 1)
                (if (> y peak) y peak)
                (+ 1 steps))))))

(define shoot (lambda (xVel yVel) (step 0 0 xVel yVel 0 0)))

(define xVelDist (lambda (x) (if (= x 1) 1
    (+ x (xVelDist (- x 1))))))

(xVelDist 6)
(shoot 7 9)
; loop through all possible ranges of X values
; loop up from zero to all hits of y

; get the max of these lists

; calculate the minimum needed velocity to even reach the target
; (shoot 3 2000)


;(define range (lambda (from to acc) (if (= from (+ 1 to)) acc
;    (range (+ from 1) to (cons (- to from) acc)))))

;(range 0 500 '())