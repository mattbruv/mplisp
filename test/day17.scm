(define xMin 20)
(define xMax 30)
(define yMin -10)
(define yMax -5)
; target area: x=288..330, y=-96..-50
;(define xMin 288)
;(define xMax 330)
;(define yMin -96)
;(define yMax -50)


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
;    (if (inBounds x y) (cons peak (cons x (cons y (cons steps '()))))
    (if (inBounds x y) peak
        ; if we are past the bounds, return FAIL
        (if (pastBounds x y) -1
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

(define xVelDist (lambda (x) (if (<= x 1) x
    (+ x (xVelDist (- x 1))))))

(define range (lambda (from to acc) (if (= from (+ 1 to)) acc
    (range (+ from 1) to (cons from acc)))))

; (xVelDist 8)
(define testRange (range -100 100 '()))
; search xs are xs whos xVelDist are inX
(define searchY (range -100 100 '()))
(define searchXs (range -100 100 '()))

(define peaks (lambda (x) (> x 0)))

(define searchX (filter (lambda (x) (inX (xVelDist x))) testRange))

(define maxAtX (lambda (x)
    (max (filter peaks (map (lambda (y) (shoot x y)) searchY)) 0)))

(define PeakAtX (lambda (x)
    (filter peaks (map (lambda (y) (shoot x y)) searchY))))

; searchX
;(max (map maxAtX searchX) 0)
;msearchX
(define length (lambda (xs count)
    (if (empty? xs) count
        (length (cdr xs) (+ 1 count)))))

(define part2 (lambda (x) (length (PeakAtX x) 0)))

;(define foo (PeakAtX 7))
;foo
;(length foo 0)
(sum (map part2 searchXs) 0)

;(shoot 7 9)
; loop through all possible ranges of X values
; loop up from zero to all hits of y

; get the max of these lists

; calculate the minimum needed velocity to even reach the target
; (shoot 3 2000)



;(range 0 500 '())