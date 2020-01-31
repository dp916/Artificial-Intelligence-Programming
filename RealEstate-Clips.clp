;Danny Pham
;CSC 180



; FAM table


(deftemplate Score
  550 750 range
  ( (Neg (600 1) (650 0))
    (Zer (600 0) (650 1) (700 0))
    (Pos (650 0) (700 1))))

(deftemplate Budget
  100000 500000 dollars
  ( (Neg (200000 1) (300000 0))
    (Zer (200000 0) (300000 1) (400000 0))
    (Pos (300000 0) (400000 1))))

(deftemplate Payment
  1 50 percent
  ( (Neg (12 1) (25 0))
    (Zer (12 0) (25 1) (37 0))
    (Pos (25 0) (37 1))))

(deftemplate Act
  0 100 confidence
  ( (BN (0 1)   (25 0))
    (N  (0 0)   (25 1) (50 0))
    (Z  (25 0)  (50 1) (75 0))
    (P  (50 0)  (75 1) (100 0))
    (BP (75 0)  (100 1))))



; Rules

(defrule credit-score
  ?i <- (go)
  =>
  (printout t "What is your credit score? ")
  (assert (credit (read)))
  (retract ?i))


(defrule fuzzify1
  (credit ?q)
  =>
  (assert (Score (?q 0) (?q 1) (?q 0))))


(defrule defuzzify1
  ?w <- (Score ?)
  =>
  (bind ?y (maximum-defuzzify ?w))
  (if (< ?y 600)
  then
  (printout t "Credit Report -> BAD " crlf)
  else
  (printout t "Credit Report -> GOOD " crlf)))



(defrule spend
  ?o <- (sec)
  =>
  (printout t "How much do you want to spend? ")
  (assert (income (read)))
  (printout t "Amount for down payment in percent: ")
  (assert (down (read)))
  (retract ?o))


(defrule report
  (credit ?c)
  =>
  (if (< ?c 600)
  then
  (printout t "You have low credit which will result in a higher interest rate. I recommend a high down payment to avoid additional costs." crlf)
  else
  (printout t "You have high credit which will result in a lower interest rate." crlf)))


(defrule house
  (income ?i)
  =>
  (if (< ?i 300000)
  then
  (printout t "Your range is between 1-3 bedroom home based on your budget." crlf)
  else
  (printout t "Your range is between 3-5 bedroom home based on your budget." crlf)))


(defrule interest
  (down ?p)
  =>
  (if (> ?p 18)
  then
  (printout t "Great choice, the higher your down payment the more money you save!" crlf)
  else
  (printout t "Be careful, the lower the down payment the longer it will take to pay off the mortgage." crlf)))



(defrule fuzzify2
  (down ?c)
  (income ?i)
  =>
  (assert (Payment (?c 0) (?c 1) (?c 0)))
  (assert (Budget (?i 0) (?i 1) (?i 0))))

(defrule defuzzify2
  (declare (salience -2))
  ?f <- (Act ?)
  =>
  (bind ?x (moment-defuzzify ?f))
  (if (>= ?x 50)
  then
  (printout t "Fuzzy: Recommends this price range. Confidence Level -> " ?x crlf)
  else
  (printout t "Fuzzy: Suggests you reconsider the costs. Confidence Level -> " ?x crlf)))



; FAM rule definition

(defrule PP
  (Payment Pos)
  (Budget Pos)
  =>
  (assert (Act BP)))

(defrule PZ
  (Payment Pos)
  (Budget Zer)
  =>
  (assert (Act P)))

(defrule PN
  (Payment Pos)
  (Budget Neg)
  =>
  (assert (Act Z)))

(defrule ZP
  (Payment Zer)
  (Budget Pos)
  =>
  (assert (Act P)))

(defrule ZZ
  (Payment Zer)
  (Budget Zer)
  =>
  (assert (Act Z)))

(defrule ZN
  (Payment Zer)
  (Budget Neg)
  =>
  (assert (Act N)))

(defrule NP
  (Payment Neg)
  (Budget Pos)
  =>
  (assert (Act Z)))

(defrule NZ
  (Payment Neg)
  (Budget Zer)
  =>
  (assert (Act N)))

(defrule NN
  (Payment Neg)
  (Budget Neg)
  =>
  (assert (Act BN)))



; Initial Facts

(deffacts second
  (sec))

(deffacts first
  (go))

 
