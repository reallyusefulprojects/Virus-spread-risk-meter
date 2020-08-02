# Virus-spread-risk-meter
Virus spreading risk indicator based on specific humidity measurement with hygrostat function

Virus risk indication meter
Really useful project by Harold de Gouw in Dutch


About this Arduino project

Eind maart 2020 volgde ik de BLOG van Maurice de Hond en kwam een interessant stuk tegen over de relatie tussen de verspreiding van het coronavirus en de specifieke luchtvochtigheid. Hij verwijst naar een zelf gemaakte pagina “Covid-19 virus verspreidingssnelheid en luchtvochtigheid” waarop hij een calculator presenteert om het verspreidingsrisico te berekenen. Om deze verspreidingsrisicometer meer praktisch bruikbaar te maken heb ik die in dit Arduino project nagebouwd, immers meten is weten.

De virusrisicometer waarschuwt bij een te lage luchtvochtigheid, waarbij uitgeademde aerosolen een hogere kans hebben om opgenomen te worden in de lucht. Deze meter meet de “relatieve luchtvochtigheid” RH, de temperatuur T en de luchtdruk P en berekent vervolgens de “specifieke luchtvochtigheid” SH (ook wel “Mixing Ratio" MR genoemd, zie als voorbeeld deze vochtigheidscalculator). Op deze SH berekening is de waarschuwingsindicatie gebaseerd. Wetenschappelijk onderzoek van M. Sajadi c.s. geeft een duidelijke relatie weer tussen het verspreidingsrisico en de specifieke luchtvochtigheid. De virusrisicometer is ook een hygrostaat. Indien de “specifieke luchtvochtigheid” SH te laag wordt, kan een relais geschakeld worden. De aanname is dat de risico-indicatie geldt voor alle via aerosolen verspreidende virussen zoals bijvoorbeeld coronavirussen en influenzavirussen. De virusmeter is een experimenteel apparaat en de waarschuwingsindicatie is gebaseerd op hypothesen (weliswaar zeer plausibel). De grenswaardes hieronder zijn daardoor niet absoluut. 


Schakelinstelling Specifieke Luchtvochtigheid, Led waarschuwings- Indicaties en acties

SH in g/kg
Relais
Led Rood: Gevaar! 
Led Geel: Let OP!
Led Groen: Veilig 
Scherm indicatie:
< 6,0
High  
High


Waarschuwing Gevaar!, Luchtvochtigheid te laag
>6,0 & <6,5
High  
High
High

Let op Gevaar dreigt! Luchtv. bijna te laag
>6,5 & <7,5
High  
High
High

Let op Gevaar dreigt! Luchtv. bijna te laag
>7,5 & <8,0
High
  
High
High
Virus verspreidingsrisico laag, Luchtvochtigheid OK
>8,0 & <8,2
High (for rising humidity) & Low (for decreasing humidity) 


High
Virus verspreidingsrisico laag, Luchtvochtigheid OK
>8,2



High
Virus verspreidingsrisico laag, Luchtvochtigheid OK




Components and supplies
Help your local supplier to survive in Corona time
-Arduino Uno R3
-Dupont line 3 kinds
-Led trafic light module
-Relais
-Sensor simple  –Sensor advanced
-Scherm simple -Screen advanced
-Breadboard simple 

-Voedingstrafo advanced
-Inlet power socket advanced
-Outlet power socket advanced
-Enclosure


Long supply time but Cheap:
-Arduino Uno R3 Clone Mega328P  €3,73
-Dupont line 3 kinds 3x40stuks €3,55
-Led trafic light module €1,80
-Relais €1,31 (not needed if you only want a virus risk indication)
-Sensor housing €4,02
-Sensor module DHT11 simple €2,27 –Sensor BME280 advanced  €2,89
-Screen LCD1602 simple1 €2,41 -Screen LCD1602 I2C, simple 2 & advanced €2,70
-Breadboard simple1 €2,73
Depending on your needs: 
-Power supply module 12V04 €1,73
-Inlet power socket €3,00
-Outlet power socket €1,67
-Enclosure €3,98


Bouwen

No Copyright, but the right to Copy! For personal use.  

Met het aankomend herfstseizoen kan het zomaar zijn dat iedereen een virusmeter zou willen hebben of zelfs nodig heeft, en zeker als u een horeca gelegenheid of andere openbare ruimte exploiteert. Zelf bouwen is nu eigenlijk nog de enige goede optie, want te koop zijn ze amper. 

Voor het bouwen van dit project kunnen verschillende microcontrollers en sensoren gebruikt worden. De meest simpele versie kan al gemaakt worden uit een standaard Arduino experimenteerkit. Die kits zijn in China al te koop voor enkele tientjes. 

In dit project worden 3 varianten genoemd afhankelijk van de wensen en beschikbare componenten. 

1. Simple version1, LCD1602, DHT11 sensor
2. Simple version2 LCD1602 I2C, DHT11 sensor
3. Advanced version LCD1602 I2C, BME280 sensor

Het verschil tussen de 1e 2 versies is dat bij de 2e versie veel minder kabels nodig zijn. De eerste 2 versies gebruiken een simpele sensor die de luchtdruk niet meet, de luchtdruk staat ingesteld op een vaste waarde. De luchtdruk is beperkt van invloed op de specifieke luchtvochtigheid maar de eerste 2 varianten zijn daardoor wel iets minder precies. Overigens zijn ze prima bruikbaar. 

Je kan bij alle 3 de versies kiezen welke functies je wel of niet wil benutten. Wil je alleen viruswaarschuwingsmeter bouwen dan heb je het relais en power sockets helemaal niet nodig en als voeding kun je dan een oude 12 volt adapter gebruiken van bijvoorbeeld een oud modem. En zelfs alleen met de sensor en de leds (zonder het LCD scherm) krijg je al een indicatie. Stel je wilt uitsluitend de luchtbevochtiger aansturen en uitsluitend de virusmeter als hygrostaat gebruiken,  dan laat je de led en lcd indicatoren achterwege. Je kan natuurlijk ook met losse leds werken maar de traffic light oplossing lijkt mij gewoon meer praktischer.

De virusmeter lijkt eigenlijk nog het meest op een weerstation, veel code komt daar ook vandaan.

De aansluitingen staan in de code, vergeet niet de juiste libraries te downloaden. 
