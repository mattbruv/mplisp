(define dieSize 100)

(define roll (lambda (times from acc)
    (if (= times 0) acc
        (roll (- times 1)
              (if (> (+ 1 from) dieSize) 1 (+ 1 from))
              (cons from acc)))))

(define getSquare (lambda (from moves)
    (if (= moves 0) from
        (getSquare (if (> from 9) 1 (+ 1 from))
                   (- moves 1)))))

(define winningScore 1000)

(define step (lambda (p1pos p2pos p1score p2score toMove rolls rollMod)
    (if (or (>= p1score winningScore) (>= p2score winningScore))
        (cons (if (< p1score p2score) p1score p2score) rolls)
        (step (if (= toMove 1) (getSquare p1pos (sum (roll 3 rollMod '()) 0)) p1pos) ;p1Pos
              (if (= toMove 2) (getSquare p2pos (sum (roll 3 rollMod '()) 0)) p2pos) ;p2Pos
              (if (= toMove 1) (+ p1score (getSquare p1pos (sum (roll 3 rollMod '()) 0))) p1score) ;p1score
              (if (= toMove 2) (+ p2score (getSquare p2pos (sum (roll 3 rollMod '()) 0))) p2score) ;p2score
              (if (= toMove 1) 2 1) ;toMove
              (+ rolls 3) ;rolls
              (if (> (+ rollMod 3) dieSize) (- (+ rollMod 3) dieSize) (+ rollMod 3))))))

(define p1 7)
(define p2 5)
(define answer (step p1 p2 0 0 1 0 1))
(define loserScore (car answer))
(define rolls (car (cdr answer)))
(* loserScore rolls)