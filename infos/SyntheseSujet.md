# Synthese du sujet 

## 1 
 <div style="text-align: justify">
Application “source” -> trafic UDP (plusieurs flux). 
1 fenêtre d’émission par flux (élém envoyés/non ACK).
Cette fenêtre gére retransmissions messages perdus. Taille fenêtre d’émission de source est dynamique (elle est le minimum taille fenêtre congestion/réception, annoncée par serveur.)
Fenêtre de congestion évolue avec temps /nombre d’acquittements reçus (ou non). 1 application “destination” reçoit/acquitte trafic.
Coupler ces applications avec ECN qui permet aux éléments du réseau de signaler qu’ils sont trop chargés. Lors de réception de ce signal, l’application “source” réduit sa fenêtre de congestion. Cette réduction réactive évite surcharge de l’élément ayant utilisé le mécanisme ECN, afin que ce-dernier ne soit pas contraint de jeter des paquets.
<br>
Vous étudierez ensuite évolution débit, des pertes/autres caractéristiques avec/sans ce mécanisme. L’implémentation des divers composants et l’étude de l’impact du mécanisme ECN seront évalués.
<div>