# LichtprojektHGS

## Projektmiglieder:

André, Adorjan, Darien

## Projektbeschreibung

Es soll für mehrere Räume eine Lichsteuerung und Rolladensteuerung geben. Für jeden Raum soll das Licht und der Rolladen einzeln ansteuerbar sein. Mit einem Taster soll in den Automatikmodus gewechselt werden können. In diesem wird die Steuerung automatisch für alle Zimmer übernommen anhand der Messung eines Fototransitors.

Die Sieben-Segment Anzeige zeigt in welchem Raum man sich bei der Steuerung befindet. Zwei Taster schalten zum nächsten oder zum vorherigen Raum. Ein Taster macht das Licht an oder aus und ein anderer schaltet den Automatik-Modus an oder aus. Zwei weitere Schalter machen die Rolladen hoch und runter. Bei einem Doppelklick auf eine Taste(zB. die Taste um den Rolladen hochzumachen) fährt dieser ganz hoch. Ansonsten muss der Taster gedrückt gehalten werden damit der Rolladen sich bewegt.

### Raumansteuerung

Jeder Raum soll ein individuell ansteuerbarens Licht und Rolladen erhalten. Mit zwei Tastern können die Räume aufwärts und abwärts gewechselt werden. Dies gibt an für welchen Raum die Ansteuerung der folgenden Features gilt. Auf der 7-Segment Anzeige wird angezeigt welcher Raum gerade verwendet wird.

### Lichtsteuerung

Mit einer Taste kann man das Licht in dem ausgewählten Raum an und aus schalten. Es gibt 8 Räume, dargestellt mit 8 LEDs. Mit der letzten Taste wird ausgewählt ob der Raum im Automatik Modus ist. Dabei wird durch ein Phototransistor entschieden ob das Licht an oder aus sein soll. Wenn es dunkel ist geht das Licht an, wenn es hell ist geht das Licht aus. Dies geschied nur in Räumen bei den der Automatik-Modus aktiviert wurde, die Räume bei denen dieser nicht eingeschaltet ist werden weiterhin über den an/aus Taster gesteuert.  

### Rolladensteuerung

Mit zwei Tastern kann gesteuert werden ob der Rollladen hoch oder herunter gefahren werden soll. Bei einem kurzen Antippen wird der Rolladen komplett herauf oder herunter gefahren. Sollte der Taster gedrückt gehalten werden bewegt sich der Rolladen nur so lange bis der Taster wieder los gelassen wird. Sollte für das Zimmer der Automatik-Modus angeschaltet sein wird der Status des Rollladens von dem Phototransistor bestimmt, sollte es dunkel sein wird der Rolladen automatisch heruntergefahren, falls nicht wird er wieder hochgefahren.
