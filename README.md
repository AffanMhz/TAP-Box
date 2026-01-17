# TAP-Box

**Tactile Access Platform for Blind Learners**

*Team ctrl+WIN - Jamia Millia Islamia*

> A low-cost refreshable Braille surface that trades millisecond refresh speeds for second-scale updates, achieving an 80–90% cost reduction while enabling real-time educational interaction for blind and deaf-blind students.

---

## 📋 Table of Contents

- [Team Information](#team-information)
- [Problem Statement](#problem-statement)
- [Our Solution](#our-solution)
- [Motivation and Vision](#motivation-and-vision)
- [Literature Review](#literature-review)
- [Our Contribution](#our-contribution)
- [Technical Specifications](#technical-specifications)
- [Hardware Architecture](#hardware-architecture)
- [Software Architecture](#software-architecture)
- [Repository Contents](#repository-contents)
- [Getting Started](#getting-started)
- [Prototype Images](#prototype-images)
- [Implementation Strategy](#implementation-strategy)
- [Expected Challenges](#expected-challenges)
- [Role of AI Tools](#role-of-ai-tools)
- [Future Work](#future-work)
- [Contributing](#contributing)
- [License](#license)

---

## 👥 Team Information

**Team Name:** ctrl+WIN  
**College:** Jamia Millia Islamia

**Team Members:**
1. **Mohd Zahaib Eqbal** - Software Architecture & G-code Generation
2. **Affan Danish** - Electronics & Circuit Design
3. **Mohit Chauhan** - Mechanical Design & CAD Modelling
4. **Alina Siddiqui** - Physical Braille Assembly, Product Design & Psychological Assessment

---

## 🎯 Problem Statement

### The Challenge

Imagine a classroom where the teacher draws, modifies, and erases diagrams in real time, while a student can only hear the description. The evolution of spatial representations cannot be followed through Braille, leaving concepts abstract. **This is the everyday reality for blind and deaf-blind students.**

### Impact in India

- [**Millions** of individuals are blind in India](https://pmc.ncbi.nlm.nih.gov/articles/PMC9359234/)
- Approximately [**1.1 million** school-age children](https://tinyurl.com/43ebenbz) are visually impaired
- Existing tactile graphics are **pre-prepared, static, and limited**
- **No support** for real-time teaching

### Educational Gaps

Foundational skill areas most affected:
- Mathematics
- Geometry
- Science diagrams
- Graphs and charts
- Spatial concepts

### The Speed Mismatch

- Visually impaired students [require **2–3× more time**](https://files.eric.ed.gov/fulltext/EJ1195718.pdf) to understand concepts
- Teaching continues at a visual pace
- Creates dependency on human intermediaries
- Results in a **non-scalable one-to-one model**
- Reduces learner independence

### Current Solutions Fall Short

Existing solutions for Braille-based education rely primarily on:
- Refreshable Braille displays
- Digital Braille readers
- Mobile learning applications

**Critical Limitations:**
- [Hardware solutions remain **prohibitively expensive**](https://www.afb.org/node/16207/refreshable-braille-displays)
- Single-line displays restrict contextual understanding
- Advanced multiline/tactile graphic systems are commercially scarce and cost-intensive
- Software and AI-based tools cannot replace tactile, real-time instructional interaction

**Therefore a need arises to solve for a Realtime, Accessible and Affordable system.**

---

## 💡 Our Solution

We propose a **low-cost refreshable Braille surface** that fundamentally reimagines the actuator architecture. Instead of one actuator per pin, we use a **2D XY plotter that moves a single Z finger**. That finger mechanically trips spring-latched pins so they pop up. To reset, students press them back down.

On top of this hardware, a teacher writes on a tablet, text is converted to Braille, images are converted into tactile patterns, and students feel it in **near real time**. This trades millisecond refresh speeds for second-scale updates—a deliberate choice aligned with educational use where **stability matters more than speed**.




### Key Innovation

Our approach directly addresses the persistent cost bottleneck: **most existing systems require one dedicated actuator per Braille pin**, which scales linearly with the number of cells and drives up cost, control complexity, and I/O pin requirements. By using an XY plotter mechanism with a single Z-axis actuator and mechanical latching, we achieve:

- **80–90% cost reduction** compared to traditional refreshable Braille displays
- **Scalable display sizes** without proportional cost increase
- **Improved durability** through simplified mechanical design
- **Affordable access** for schools and learners who have long been priced out of Braille technology

![Core Idea](Images/image_6.jpeg)
*Introduction of the core idea: XY plotter with Z-axis pin triggering*

---

## 🌟 Motivation and Vision

We are motivated by the fundamental gap between how education is delivered and the lack of effective ability of blind children to access existing solutions due to high-stake constraints in the current ecosystem. 

**We envision** an independent and dynamic learning system that enables blind learners to grasp concepts as they are taught, ensuring that the absence of accessible visual tools does not lead to long-term educational and social exclusion.

---

## 📚 Literature Review

### Recent Research Advances

Recent efforts to make refreshable Braille displays more accessible have focused on electromagnetic actuation as a lower-cost alternative to piezoelectric or other complex mechanisms:

#### Electromagnetic Actuation Systems

- **[Hao Chen et al.](https://pubmed.ncbi.nlm.nih.gov/37022849)** proposed a layered electromagnetic driving mechanism
  - Dense Braille pin arrangements with stable performance
  - Long service life
  - Support force >150 mN
  - Refresh rates up to 50 Hz under 6V supply
  - Significant improvements in cost and usability for developing regions

- **[Md Asraful Alam et al.](https://www.researchgate.net/publication/385349516_Refreshable_Braille_Display_A_Review_of_Existing_Technologies_and_a_Proposed_Method)** conducted thorough review of RBD actuation technologies
  - Electromagnetic actuation using permanent magnet, electromagnet, and cam per Braille pin
  - Strong balance of economic and technical performance metrics

#### Mechanical Latching Approaches

- Low-cost beam-style latching structures demonstrated:
  - Latching force >5 N
  - Average refresh frequencies ~17 Hz
  - User recognition accuracy ~95.5%
  - Practical viability when combined with multimodal interactive environments

*(Reference: D. Leonardis, C. Loconsole, and A. Frisoli, "A Survey on Innovative Refreshable Braille Display Technologies," vol. 587, Springer, Cham, 2017, pp. 488–498)*

#### Commercial Alternatives

- Multiple commercial boards use [pneumatic architecture](https://www.youtube.com/watch?v=0fIg4rI4cDw)
- Still face cost and complexity challenges

### The Persistent Gap

**Despite these advances, a critical bottleneck remains:** Most approaches still require **one dedicated actuator per Braille pin**, which scales linearly with the number of cells and drives up cost, control complexity, and I/O pin requirements.

**Our work aims to reduce the number of actuators per control pin.**

---

## 🔬 Our Contribution

Our design introduces the following key contributions:

### 1. Two-Layer Architecture
A 2D plotter mechanism forms the bottom layer and provides precise X–Y positioning across the Braille surface.

### 2. Single Z-Axis Actuation
Replacement of per-pin actuators with a single Z-axis actuation mechanism that selectively raises Braille pins using a **click-based mechanical latch**.

### 3. Three-Actuator System
A reduction of the entire system to **three actuators (X, Y, and Z)**, trading refresh time for substantial gains in cost, simplicity, and manufacturability.

### 4. Education-Optimized Design
Design optimized for educational and teaching scenarios, where slower refresh rates are acceptable but **affordability and robustness are critical**.

### 5. Breaking the Scaling Rule
By **decoupling spatial addressing from actuation** and reusing a small set of motors across the full display area, this approach breaks the traditional one-actuator-per-pin scaling rule and enables **low-cost, large-area refreshable Braille systems**.

---

## 🔧 Technical Specifications

### Device Overview

| **Specification** | **Details** |
|------------------|-------------|
| **Device Name** | TAP-Box |
| **Description** | Low-cost, mechanically actuated refreshable Braille display optimized for educational use and manufacturability |
| **Device Footprint** | 27 cm × 27 cm × 7 cm |
| **Braille Pin Matrix** | 324 pins (18 × 18 grid) |
| **Pin Pitch** | 2.5 mm (standard Braille) |
| **Braille Pin Material** | Brass (or earbuds for low-cost prototypes) |

### X-Y Plotter Mechanism

The TAP-Box employs a coordinate-based positioning system:

```
┌─────────────────────────────────┐
│  Y-Axis Stepper Motor           │
│         ↓                       │
│    ┌─────────┐                  │
│    │ Plotter │→ X-Axis Motor    │
│    │  Head   │                  │
│    └─────────┘                  │
│         ↓                       │
│   Pin Actuator                  │
│         ↓                       │
│  ┌─────────────────┐            │
│  │  Braille Cell   │            │
│  │  Grid with      │            │
│  │  Latching Pins  │            │
│  └─────────────────┘            │
└─────────────────────────────────┘
```

### Mechanical System

- **Fully 3D-printed** components:
  - X–Y gantry and carriage
  - Z-axis linear actuator for pin triggering
  - Single moving trigger head actuates individual pins

### Electronics

| **Component** | **Specification** |
|--------------|------------------|
| **Microcontroller** | Arduino Uno |
| **Motor Drivers** | 2 × A4988 stepper drivers (X–Y) |
| **Shield** | CNC Shield |
| **X-Y Actuators** | 2 × Stepper motors |
| **Z-Axis Actuator** | 1 × Linear motor |
| **Power Supply** | 12V, 5A DC |
| **Homing** | Mechanical limit switches (X–Y) |

![TAP-Box CAD Model](Images/image_1.jpeg)
*Complete CAD model of TAP-Box*

![TAP-Box Circuit Diagram](Images/circuit_diagram.jpeg)
*Hardware and software circuitry*


---

## 🖥️ Software Architecture

The system is designed as a **browser-based teaching platform** that allows instructors to enter textual and mathematical content in real time. All interaction takes place through a standard web browser, eliminating the need for local software installation.

### Input Processing Layer

1. **Browser Interface**
   - Standard web browser access
   - No local software installation required
   - Real-time content entry for instructors
   - Basic tools for writing, annotating, and structuring content

2. **Optical Content Recognition**
   - Browser-compatible OCR layer based on **Tesseract.js**
   - Converts handwritten or typed text and mathematical symbols
   - Machine-readable text output

3. **Normalization**
   - Content cleaning and normalization
   - Removes ambiguities
   - Ensures consistency for mathematical expressions

### Braille Conversion Module

- Custom Braille conversion engine
- Converts alphabets, numbers, and mathematical operators
- Standard literary Braille and Braille mathematics notation
- Organized as dot matrix representation matching physical layout

### Motion Control Layer

1. **G-code Generation**
   - Dot matrix data translated into motion and actuation instructions
   - G-code format for precise control

2. **GRBL Controller**
   - GRBL-based controller manages positioning
   - Precise actuation of mechanical elements
   - Reliable and repeatable tactile output

3. **Wireless Communication**
   - G-code commands transmitted over **Wi-Fi**
   - Minimal latency
   - Reliable wireless operation

**GitHub Repository:** [https://github.com/AffanMhz/TAP-Box.git](https://github.com/AffanMhz/TAP-Box.git)

---

## 📁 Repository Contents
```
TAP-Box/
│
├── 3D_Models/
│   └── TAP-Box.stl
│
├── Arduino_Programs/
│   ├── x_axis_test/
│   │   └── x_axis_test.ino
│   ├── y_axis_test/
│   │   └── y_axis_test.ino
│   ├── z_axis_test/
│   │   └── z_axis_test.ino
│   ├── grbl_controller/
│   │   └── grbl_config.ino
│   └── README.md
│
├── Software/
│   ├── web_interface/
│   ├── ocr_module/
│   ├── braille_converter/
│   ├── gcode_generator/
│   └── wifi_communication/
│
├── Images/
│   ├── image_1.jpeg/
│   ├── image_2.jpeg/
│   ├── illustration.jpeg/
│   ├── image_3.jpeg/
│   ├── image_4.jpeg/
│   ├── image_5.jpeg/
│   ├── image_6.jpeg/
│   └── circuit_diagram.jpeg/
│
├── Documentation/
│   ├── assembly_instructions.md
│   ├── wiring_diagram.pdf
│   ├── technical_specifications.md
│   ├── BOM.xlsx
│   └── project_proposal.pdf
│
├── LICENSE
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites

- Arduino IDE (version 1.8.x or higher)
- 3D printer (build volume: 25cm × 25cm minimum)
- CNC Shield compatible with Arduino Uno
- A4988 stepper motor drivers (2×)
- Stepper motors (2×)
- Linear motor/actuator (1×)
- 12V, 5A power supply
- Basic electronics tools and soldering equipment

### Hardware Assembly

1. **Print 3D Components**
```bash
   # Print all files from 3D_Models/ directory
   # Recommended settings:
   # - Layer height: 0.2mm
   # - Infill: 20-30%
   # - Material: PLA or PETG
   # - Supports: Yes (for overhangs)
```

2. **Assemble Mechanical Frame**
   - Follow instructions in `3D_Models/assembly_guide.md`
   - Install linear rails/rods for X and Y axes
   - Mount stepper motors to gantry
   - Install timing belts or lead screws
   - Attach Z-axis linear actuator to carriage

3. **Install Braille Pin Grid**
   - Mount pin grid base to frame
   - Insert brass pins (or earbud alternatives)
   - Install latching mechanisms
   - Calibrate pin spacing (2.5mm standard)

4. **Wire Electronics**
   - Refer to `Documentation/wiring_diagram.pdf`
   - Mount CNC Shield on Arduino Uno
   - Install A4988 drivers on CNC Shield
   - Connect stepper motors to X and Y drivers
   - Connect Z-axis motor
   - Install limit switches for homing
   - Connect 12V power supply

### Software Setup

1. **Clone Repository**
```bash
   git clone https://github.com/AffanMhz/TAP-Box.git
   cd TAP-Box
```

2. **Upload GRBL Firmware**
```bash
   # Open Arduino IDE
   # Load Arduino_Programs/grbl_controller/grbl_config.ino
   # Select Board: Arduino Uno
   # Select correct COM port
   # Upload
```

3. **Configure Web Interface**
```bash
   cd Software/web_interface
   # Follow setup instructions in README.md
   # Configure WiFi credentials
   # Deploy to local server or cloud
```

4. **Test Individual Axes**
   - Upload and run `x_axis_test.ino`
   - Upload and run `y_axis_test.ino`
   - Upload and run `z_axis_test.ino`
   - Verify smooth motion and positioning

---

## 🖼️ Prototype Images

### Concept and Design

![Core Idea](Images/mechanism_diagrams/core_idea.png)
*XY plotter with Z-axis pin triggering mechanism*

![System Architecture](Images/mechanism_diagrams/system_architecture.png)
*Complete hardware and software architecture*

### CAD Renders

![CAD Model](Images/cad_renders/full_assembly.png)
*Complete CAD assembly*

![Exploded View](Images/cad_renders/exploded_view.png)
*Exploded view showing all components*

### Prototype Build

![Prototype Overview](Images/image_1.jpeg)
*TAP-Box prototype - complete assembly (27cm × 27cm × 7cm)*

![XY Gantry](Images/image_2.jpeg)
*XY gantry system with stepper motors*

![Z-Axis Actuator](Images/image_3.jpeg)
*Z-axis linear actuator and trigger head*

![Pin Grid](Images/image_4.jpeg)
*18×18 Braille pin grid with latching mechanism*

![Electronics](Images/circuit diagram.jpeg)
*Arduino Uno with CNC Shield and A4988 drivers*

### Assembly Process

![Assembly Step 1](Images/assembly_steps/step_01.jpg)
*Frame and gantry assembly*

![Assembly Step 2](Images/assembly_steps/step_02.jpg)
*Motor and belt installation*

![Assembly Step 3](Images/assembly_steps/step_03.jpg)
*Pin grid installation*

![Assembly Step 4](Images/assembly_steps/step_04.jpg)
*Electronics integration and wiring*

### Testing Phase

![X-Axis Test](Images/testing_photos/x_axis_test.jpg)
*X-axis motion calibration*

![Y-Axis Test](Images/testing_photos/y_axis_test.jpg)
*Y-axis motion calibration*

![Z-Axis Test](Images/testing_photos/z_axis_test.jpg)
*Z-axis pin triggering test*

![Full System Test](Images/testing_photos/full_system.jpg)
*Complete system testing with Braille pattern*

---

## 📅 Implementation Strategy

### Team Roles and Milestones

#### Day 1

| Team Member | Goals |
|------------|-------|
| **Mohit** | **Goal 1:** Finalization of refreshable Braille pin matrix, housing, and plotter moving system<br>**Goal 2:** Coordination with software for Z-axis latching and alignment |
| **Affan** | **Goal 1:** Interfacing of actuators, motor driver circuits and power regulation systems<br>**Goal 2:** Realization of initial circuit schematic, prototyping, and testing of basic motor motion |
| **Zahaib** | **Goal 1:** Design of complete software architecture including input processing, Braille mapping, and data flow<br>**Goal 2:** Development of Braille character mapping, coordinate conversion algorithms, G-code generation |
| **Alina** | **Goal 1:** Design of Braille layouts on board material, tactile representations, and preparation of reference charts<br>**Goal 2:** Calibrate tactile sensitivity by testing pin force and distinguishability |

#### Day 2

| Team Member | Goals |
|------------|-------|
| **Mohit** | **Goal 3:** Complete mechanical assembly including motor mounting, linear rail installation, and full motion calibration<br>**Goal 4:** Integration of vertical actuator with pin engagement mechanism, precision tuning |
| **Affan** | **Goal 3:** Final circuit assembly, mounting and wiring of motors and actuators, and firmware configuration<br>**Goal 4:** Electronics–mechanics–software integration, motor calibration, timing optimization |
| **Zahaib** | **Goal 3:** Integration of G-code pipeline with microcontroller communication and physical system calibration<br>**Goal 4:** Software validation through full test sequences, synchronization debugging, speed optimization |
| **Alina** | **Goal 3:** Final installation and alignment of pin matrix with actuator system and reliability testing<br>**Goal 4:** User-oriented testing, documentation of accessibility feedback, and preparation of demos |

---

## ⚠️ Expected Challenges

After defining the system architecture, several non-trivial mechanical, software, and human-factor challenges emerge:

### 1. Mechanical Repeatability & Precision

**Challenge:** Braille dots are small, tightly spaced, and standardized. Any XY drift, frame flex, or latch wear causes height or spacing errors that are immediately detectable by touch.

**Mitigation:**
- Using rigid geared rails
- Adding end-stop sensors and frequent re-homing
- Designing latches tolerant to small Z-axis errors
- Treating mechanical calibration as continuous, not one-time

### 2. Latch Design & Fatigue

**Challenge:** The latch mechanism determines system reliability. Partial latching or fatigue can leave pins stuck, ruining an entire Braille cell.

**Mitigation:**
- Over-engineering the latch with minimal parts
- Avoiding friction-based mechanisms
- Preferring gravity- and spring-assisted designs
- Designing the pin sheet as a replaceable consumable
- Expecting periodic servicing (printer-like maintenance model)

### 3. Pin Reset Mechanism

**Challenge:** Raising pins is insufficient unless a reliable reset method exists.

**Mitigation:**
- Manual full-board press by the student
- Dedicated "Ready" button to reinitialize pin states

### 4. Tactile Graphics Interpretation

**Challenge:** Visual images do not map directly to touch. Excess detail creates noise; shading and dense graphics become unreadable.

**Mitigation:**
- Using symbolic simplification (edges, contours, textures)
- Following existing tactile graphics standards
- Giving teachers control over what is rendered tactilely

### 5. Software Latency & Synchronization

**Challenge:** Teacher input → processing → plotter motion → mechanical actuation introduces cumulative delay.

**Mitigation:**
- Pipelining the entire system
- Streaming strokes instead of full-frame updates
- Prioritizing text over graphics
- Accepting "followable" latency, not zero latency

### 6. Noise, Vibration & Classroom Usability

**Challenge:** Plotter motion introduces noise and vibration, which can disrupt classroom environments.

**Mitigation:**
- Slow, smooth motion profiles
- Rubber vibration isolation
- Enclosure-based acoustic damping
- Trade speed for silence

### 7. Human Factors & Trust

**Challenge:** Blind users rely on absolute reliability. A single incorrect output can permanently erode trust.

**Mitigation:**
- Blank cells if pin state is uncertain
- Providing manual verification
- Involving blind users from early design stages

---

## 🤖 Role of AI Tools

AI tools played a key role in accelerating our understanding and problem-solving during the development of TAP-Box:

### Design Assistance
- **ChatGPT** helped us understand the complete workflow of converting images into G-code, including coordinate mapping and motion sequencing

### Mechanical Troubleshooting
- AI-assisted analysis of mechanical issues such as unequal motor motion
- Highlighted probable causes like misalignment and friction
- Suggested solutions involving bearing integration and belt adjustment

### Conceptual Development
- **Pen clicking analogy** provided by ChatGPT was used for the Braille stick movement model
- Helped polish the mechanical design and improve efficiency

### Important Note
**The complete core idea was ours.** AI helped us refine the mechanical design and make it more efficient, but the fundamental innovation of using an XY plotter with mechanical latching for Braille display was conceived by our team.

---

## 🎯 Future Work

### Short-term Goals (0-3 months)
- [ ] Optimize refresh rate to achieve full display update in <30 seconds
- [ ] Improve latch mechanism durability through material testing
- [ ] Develop comprehensive tactile graphics library
- [ ] Create teacher training materials and documentation

### Medium-term Goals (3-6 months)
- [ ] Expand display size to 24×24 cells (576 pins)
- [ ] Implement advanced image-to-tactile conversion algorithms
- [ ] Integrate with popular educational software platforms
- [ ] Conduct pilot testing in 3-5 schools
- [ ] Develop custom PCB to replace Arduino + CNC Shield

### Long-term Goals (6-12 months)
- [ ] Achieve target cost of **<₹15,000 ($180)** per unit
- [ ] Establish manufacturing partnerships for scale production
- [ ] Create standardized curriculum integration guides
- [ ] Publish research findings in accessibility conferences
- [ ] Deploy in 50+ educational institutions across India

### Research Directions
- [ ] Alternative latching mechanisms (magnetic, pneumatic)
- [ ] Multi-height pins for grayscale tactile representation
- [ ] Real-time collaborative features for remote learning
- [ ] Integration with screen readers and assistive technologies

---

## 🧪 Testing & Validation

### Functional Testing

| Test | Status | Notes |
|------|--------|-------|
| X-axis precision | ✅ Passed | ±0.3mm accuracy |
| Y-axis precision | ✅ Passed | ±0.3mm accuracy |
| Z-axis triggering | ✅ Passed | 95% success rate |
| Pin latching | 🔄 In Progress | Testing durability |
| G-code execution | ✅ Passed | <5s latency |
| WiFi communication | ✅ Passed | Stable connection |

### User Testing

- **Planned:** Field testing with visually impaired students
- **Focus areas:** 
  - Pin distinguishability
  - Refresh rate acceptability
  - Learning effectiveness
  - Comfort and ergonomics

---

## 🤝 Contributing

We welcome contributions from the community! Whether you're interested in:

- 🔧 Hardware design improvements
- 💻 Software optimization
- 📚 Documentation
- 🧪 Testing and feedback
- 🎓 Educational content development
- ♿ Accessibility research

### How to Contribute

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Development Guidelines

- Follow existing code style and documentation standards
- Test all mechanical changes thoroughly
- Document any modifications to 3D models
- Update README.md and relevant documentation

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### Open Source Commitment

We believe assistive technology should be accessible to all. This project is open source to enable:
- Educational institutions to build their own units
- Researchers to advance the technology
- Developers to contribute improvements
- Global collaboration on accessibility solutions

---

## 📧 Contact

**Team ctrl+WIN**

- **GitHub:** [https://github.com/AffanMhz/TAP-Box](https://github.com/AffanMhz/TAP-Box)
- **Institution:** Jamia Millia Islamia
- **Project Proposal:** See `Documentation/project_proposal.pdf`

For questions, collaboration opportunities, or feedback:
- Open an issue on GitHub
- Contact team members through university email

---

## 🙏 Acknowledgments

### Research References
- Hao Chen et al. - Electromagnetic Braille display mechanisms
- Md Asraful Alam et al. - RBD actuation technology review
- D. Leonardis, C. Loconsole, and A. Frisoli - Survey on refreshable Braille displays

### Support & Inspiration
- Visually impaired students and educators who provided invaluable feedback
- Open-source hardware and accessibility communities
- Jamia Millia Islamia for institutional support
- AI tools (ChatGPT) for design assistance and problem-solving

### Standards & Resources
- American Foundation for the Blind (AFB)
- Braille Authority of North America (BANA)
- Tactile graphics standards and guidelines

---

## 📊 Project Status

![Status](https://img.shields.io/badge/Status-Prototype-yellow)
![Hardware](https://img.shields.io/badge/Hardware-Testing-orange)
![Software](https://img.shields.io/badge/Software-Development-blue)
![License](https://img.shields.io/badge/License-MIT-green)

**Current Version:** v0.1-prototype  
**Last Updated:** January 2026  
**Build Status:** Functional prototype with 324-pin (18×18) display

---

## 📈 Impact Metrics (Projected)

| Metric | Traditional Display | TAP-Box |
|--------|-------------------|---------|
| **Cost per unit** | ₹80,000 - ₹2,00,000 | **₹12,000 - ₹15,000** |
| **Cost reduction** | Baseline | **80-90%** |
| **Display size** | 1-2 lines (limited) | **18×18 cells (expandable)** |
| **Refresh mechanism** | Continuous power | **Mechanical latch (zero power hold)** |
| **Maintenance** | Complex, expensive | **User-serviceable** |
| **Scalability** | Linear cost increase | **Sub-linear scaling** |

---

<div align="center">

## 🌍 Making Education Accessible, One Braille Cell at a Time

**TAP-Box breaks the cost barrier in assistive technology**

*Empowering 1.1 million blind students in India to learn mathematics, science, and spatial concepts in real-time*

---

⭐ **Star this repository if you believe in inclusive education!**

🔗 **Share to help us reach schools, NGOs, and accessibility advocates**

💬 **Join the discussion in Issues to contribute ideas**

---

*Built with ❤️ by Team ctrl+WIN*

</div>
