# **Alarm Manager System**
## ***By Aroop, Mithun & Theophilus***

### Steps to run the code
1. Clone the repository into a directory using\
`git clone https://github.com/aroop007/AlarmManagerSystem.git`
2. Open Momentics IDE
3. Select the project location and click next
4. Click on file and click on open project
5. Select the cloned directory and import it
6. Setup target system (Either virtual target system or host system)
7. Right click on the project directory in **Project Explorer** and click on **Clean Project**
8. Then click on **Build Project**
9. Once the **Build Complete** message is printed in the terminal, Click on **Run** to run the code
10. A simple **CLI** is displayed in the terminal

### Commands Used
1. `add` It is used to create a new alarm.\
**Syntax:**  
`add`\
`number_of_seconds`\
`periodic_or_oneshot`\
**ex:**  
`add`\
`10`\
`0`\
2. `list` It is used to list all the alarms that are created.\
**Syntax:**  
`list`
3. `remove` It is used to remove or delete an alarm.\
**Syntax:**  
`remove`\
`alarm_id`\
**ex:**  
`remove`\
`2`
4. `exit` It is used to terminate or exit the program.\
**Syntax:**  
`exit`

### Client Server Feature
1. First the code in this repository should be run in one **system**.
2. Then run the **client.cpp** on another device.  
***Note: Both the devices should be connected to the same network and same target device. Delete or move the client.cpp while running on server side system.***
