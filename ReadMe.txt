All logic are written in c++ since its preferable. Blueprints are derived from it for visual and to specify default values.

Task 1:

Added a c++ class named InteractiveWidget and a blueprint class derived from it named WBP_InteractiveWidget 

Button:
The button text is formed by combining button text variable and a float which is incremented as the ability key E is pressed(Bonus Task)
When the button is pressed, an event is broadcasted and the widget itself will handle the event and toggle the label text between on and off state

Progress bar:
A progress bar is added to show health changes

Label textblock:
The text should alter between "ButtonToggle On" and "ButtonToggle Off" when button is pressed

Screen resolution and ratio:
The ui should be anchored to the middle of the screen for various screen resolutions

Visibility: 
The widget is created on the character and initially hidden. Usually UI would be on controller but in this case since its tied to an ability and the ASC i have setup is tied to the character, i think its approriate to have the widget being created in the character class.

Common UI: 
I have not used common UI before this. I would like to try but i would rather utilize it in a correct manner rather than learning in a short span and risk misusing it. If i join the project and the project is using common UI,
i can just learn from the current working and continue the correct/tried practice for implementing.



Task 2:

Added a c++ class named ShowWidgetAbility and a blueprint derived from it named GA_ShowWidgetAbility

Input command:
When E is pressed, the ability is activated and when E is released the ability is ended. The release event is controlled by an ability task which is an asynchronous operation.
When the ability is activated or ended, the widget would toggle between visible and hidden. I think toggling visibility is significantly more friendly to performance than constantly rebuilding and removing the widget therefore i chose to do this rather than adding and removing the widget from viewport

Health Reducing gameplay effect:
I added two attributes health and maxhealth to the character.
A health reducing GE is applied and removed as the ability activates or ends.
The GE period is set to 0.5 and the amount reduced can be set within the ability. We can choose to specify the amound reduced in the GE too but usually values like these are dependant upon ability level/events/different ability so i opt to made it set by caller.


Bonus task:

Task 1:
When the ability is activated, a timer of 0.1 interval is activated and the time passed is accrued within a variable in the WBP_InteractiveWidget. The variable is then displayed as text in button text. 
When the ability ends, the timer stops and the time passed variable is cleared.

Task 2:
The WBP_InteractiveWidget has a listener event tracking health attribute changes. When the health attribute change, the progress bar will reflect the changes.

