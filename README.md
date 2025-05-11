# Hospital Management System

## Overview
A console-based C++ application for managing patient flow in a small hospital environment.  
This system provides an efficient way to manage patient admissions, transfers, and discharges across multiple wards using linked list data structures.

## Features

### 1. Patient Admission
- Automatically assigns patients to available beds  
- Creates new wards when all existing wards are at full capacity  
- Tracks patient name and medical condition  

### 2. Patient Discharge
- Removes patients from the system  
- Automatically shifts remaining patients to maintain no empty beds between occupied ones  
- Validates bed index before discharge  

### 3. Patient Transfer
- Moves patients between beds  
- Handles the reorganization of patients to maintain the no-empty-beds requirement  
- Validates source and target beds before transfer  

### 4. System Display
- Shows the current status of all wards  
- Lists patients in each ward with their bed assignments  

### 5. Patient Information Retrieval
- Provides patient name and condition based on bed index  

## Usage Instructions

- Initializing the System
`hospitalManagementSystem hospital;`

- Admitting a Patient
`hospital.admitPatient("Patient Name", "Medical Condition");`

- Discharging a Patient
`hospital.dischargePatient(bedIndex);  // bedIndex is the global index of the bed`

- Transferring a Patient
`hospital.transferPatient(sourceBedIndex, targetBedIndex);`

- Displaying System Status
`hospital.displaySystem();`

- Getting Patient Information
`std::string info = hospital.getPatientInfo(bedIndex);`

## Implementation Details
- Uses linked lists to manage beds and wards
- Each bed has an occupancy status (occupied/vacant)
- Global indexing system for beds across all wards
- Automatic patient shifting to maintain continuous occupancy

## Error Handling
The system validates operations and provides appropriate error messages for:
- Attempting to discharge patients from empty or non-existent beds
- Attempting to transfer patients to occupied beds
- Attempting to transfer from empty or non-existent beds
