# Exam Personal Projects

For the personal project to be presented at the exam, you must start form the codebase at the end of the course, and implement some new feature of extension.

**NOTE**: "Personal" means also "individual". You must work alone on the project, and you must not share your code with other students. You can ask for help to the professor and the teaching assistants.

## General rules

The project must be implemented on a **private repository** of the codebase, shared only with the professor and the teaching assistants. 

The first step must be the **contract**: a document describing the new feature or extension, and how it will be implemented. The contract must be written in the `CONTRACT.md` file and approved by the professor before starting the implementation (you shaould ask the professor for a mersonal meeting to discuss the contract).

The second step is the **implementation**: you must implement the new feature or extension in the codebase, and document it in the `IMPLEMENTATION.md` file. You must produce regular commits to the repository, and write a commit message describing the changes made in each commit. The `IMPLEMENTATION.md` file must be organized in section and shall be clear and concise. A possible sections layout is:

* **Rationale** --- a brief description of the adopted strategy to implement the new feature or extension, and the reasons behind it.
* **Implementation details** --- a detailed description of the implementation, including the main challenges faced and how they were overcome.
* **Testing** --- a description of the testing strategy adopted, and the results obtained from the tests.
* **Steps**
  1. a description of the first step of the implementation, including the code changes made, the reference commit, and the rationale behind them.
  2. a description of the second step of the implementation, including the code changes made, the reference commit, and the rationale behind them.
  3. etc.


## Use of AI tools

You are allowed to use AI tools (as Copilot or Codex) to help you with the implementation, but you must **cite** the tools used and the specific prompts (verbatim!) in the `IMPLEMENTATION.md` file (in the *Steps* section). You must also **explain** how the AI tool was used and how it contributed to the implementation.

## Evaluation criteria

The project may have three outomes:

* **Approved, 30/30** --- the project has been implemented correctly and with good quality and effort. This means that the contract has been respected, the implemented code compiles with no warnings, the executable runs without errors, and the new feature or extension works as expected. The implementation is well documented in the `IMPLEMENTATION.md` file, with clear and concise descriptions of the rationale, implementation details, testing strategy, and steps taken.
* **Approved, 30/30 cum laude** --- the project has been implemented as above and with exceptional quality, effort and/or innovation.
* **Delayed/Rejected** --- the project does not meet the approval criteria. The student must revise and retry.

The project must be completed within the **end of the winter exam session 2027**.

# Features

At least two of the following features must be implemented:

## Tool Radius Compensation

Implement the ISO G-code commands `G40`, `G41` and `G42` for tool radius compensation. This feature allows the CNC machine to compensate for the radius of the cutting tool, ensuring that the desired dimensions of the workpiece are achieved. Follow the theoretical guideline on the slides (in Moodle).

## Look Ahead feature

Implement a look-ahead feature that allows the CNC machine to anticipate the next movements and adjust its speed and acceleration accordingly. This feature can improve the performance of the machine and reduce the machining time. Look ahead can be enabled/disabled with the command `M11/M10`. Follow the theoretical guideline on the slides (in Moodle).

## Tool preset table

Implement a tool preset table that allows the CNC machine to store and recall the dimensions and properties of different cutting tools. The table must be implemented as a section in the mads.ini. The G-Code command `Tn` loads the tool with index `n` from the preset table and updates the tool length compensation in the Rerun viewer and, if implemented, in the Tool Radius Compensation feature.

Example of the tool preset table in the mads.ini:

```toml
[machinetool]
#...

# Tool preset table
[[machinetool.tool_preset_table]] # this is the first tool preset
name = "tool_1"
radius = 5.0
length = 100.0 

[[machinetool.tool_preset_table]] # this is the second tool preset
name = "tool_2"
radius = 10.0
length = 150.0
```

## Workpiece coordinate system

Implement the ISO G-code commands `G54`--`G59` for workpiece coordinate system. This feature allows the CNC machine to define multiple coordinate systems for the workpiece, which can be useful for machining complex parts or for using different fixtures.

Each coordinate system must be defined in the `mads.ini` file with three translations and a rotation about Z (in degrees). The G-Code command `G5n` (modal!) selects the coordinate system with index `n` (where `n` is between 4 and 9)

The command `G53` (**NOT MODAL!**) selects the machine coordinate system for the current block.

## Relative coordinate system

Implement the ISO G-code commands `G90` and `G91` for relative coordinate system. This feature allows the CNC machine to interpret the coordinates in the G-Code commands as either absolute or relative to the current position of the tool. The command `G90` sets the absolute coordinate system, while the command `G91` sets the relative coordinate system. Both commannds are modal, meaning that they affect the interpretation of the coordinates in all subsequent G-Code commands until another command changes the coordinate system.

## Manual control mode

Implement a manual control mode that allows the operator to control the CNC machine manually using an axis selector and the up/down keys. selected axis moves in steps (both th the FMU and in the viewer). Once in position, the key `p` can be used to save the current position as workpiece origin. If the workpiece coordinate system feature is implemented, the saved position must be used as the origin of the currently selected coordinate system.

## Further features

You can propose in the contract other features or extensions that you find interesting and challenging, as long as they are related to CNC machining and are approved by the professor.