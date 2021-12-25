
(define filter-proxy (lambda (fn xs acc)
    (if (empty? xs) acc
        (if (fn (car xs))
            (filter-proxy fn (cdr xs) (cons (car xs) acc))
            (filter-proxy fn (cdr xs) acc)))))

(define filter (lambda (fn xs) (filter-proxy fn xs '())))

(define map-proxy (lambda (fn xs acc)
    (if (empty? xs) acc
        (map-proxy fn (cdr xs) (cons (fn (car xs)) acc)))))

(define map (lambda (fn xs) (map-proxy fn xs '())))