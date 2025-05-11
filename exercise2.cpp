#include <iostream>
#include "list.h"


struct Patient 
{
    std::string name;
    std::string condition;

    Patient (){};

    Patient(std::string param_name, std::string param_condition) 
    {
        name = param_name;
        condition = param_condition;
    }
       
};

class Bed 
{   private:
        Patient* patient;
        bool occupied;

    public: 
        Bed() 
        {
            patient = nullptr;
            occupied = false;
        } 
        
        void assignPatient(Patient* p) 
        {
            patient = p;
            occupied = true;
        }

        void dischargePatient() 
        {
            patient = nullptr;
            occupied = false;
        }

        bool isAvailable()
        {
            return !occupied;
        }
    
        Patient* getPatient() 
        {
            return patient;
        }

        void displayPatient() 
        {
            if (occupied) 
            {
                std::cout << patient->name;
            }
        }

};

// struct for the global index of beds - has an index of the bed and a pointer to the bed
struct GlobalIndex 
{
    int index;
    Bed* bed;
};

class Ward 
{
    private:
        LinkedList <Bed> beds;
        int bed_number; //maximum 4 in our case 
    
    public:
        int wardIndex;

        Ward(){}

        Ward(int ward_index, int numBeds, LinkedList <GlobalIndex> &listBeds) 
        {
            int lastIndex; //index of last bed
            this->wardIndex = ward_index;

            if (listBeds.plast != nullptr) 
            {
                lastIndex = listBeds.plast->info.index; // obtain the last index from the global index list
            }
            else 
            {
                lastIndex = -1; //means we have no beds in the hospital yet
            }
            // initialize the ward with a certain number of beds
            for (int i = 1; i <= numBeds; i++) 
            {
                lastIndex++;
                beds.addLast(Bed()); // add a new bed to the ward
                GlobalIndex global_index = {lastIndex, &beds.plast->info}; // create an instace of the global index (bed index and pointer to the bed)
                listBeds.addLast(global_index); // add the global index to the global index list

            }
            bed_number = numBeds;
        }
        
        // function finds the first available bed in the ward and assigns a patient to it
        Node<Bed>* assignPatientToBed(Patient* p) 
        {
            Node<Bed>* bedPtr = beds.pfirst;
            while (bedPtr != nullptr) // if bed exists
            {
                Bed bed = beds.getInfo(bedPtr);
                if (bed.isAvailable()) 
                {
                    bed.assignPatient(p);
                    bedPtr->info = bed; // update the bed in the list (since its occupancy status changed)
                    return bedPtr;
                }
                bedPtr = bedPtr->next; // move to the next bed
            } 
            return nullptr; // returns this if all beds are occupied in the ward
        }


        // function to display all names of patients in the ward
        void listPatients() 
        {
            bool first_patient_printed = false; // this flag is needed to print the first patient without a comma
            Node<Bed>* bedPtr = beds.pfirst;
            while (bedPtr != nullptr) 
            {
                Bed bed = beds.getInfo(bedPtr);
                if (!bed.isAvailable()) 
                {
                    if (first_patient_printed) 
                    {
                        std::cout << ", ";
                    }
                    bed.displayPatient();
                    first_patient_printed = true;   
                }
                bedPtr = bedPtr->next;
            }
        }
        // function to display the ward information and the patients in it
        void display() {
            std::cout << "Ward №" << wardIndex << " with " << bed_number << " beds: ";
            listPatients();
            std::cout << std::endl;
        }
};

class hospitalManagementSystem 
{
    private:
        LinkedList <GlobalIndex> IndexBeds; // linked list of global indexes of beds (bed index and pointer to the bed)
        LinkedList <Ward> wards;
        int wardCount = 0;

    public:
        hospitalManagementSystem() 
        {
            wardCount ++;
            wards.addLast(Ward(wardCount, 4, IndexBeds)); // 4 beds in the ward
        }

        // function to list all wards with patients
        void displaySystem () 
        {
            std::cout << "List of Wards:" << std::endl;
            Node<Ward>* wardPtr = wards.pfirst;
            
            while (wardPtr != nullptr) // display all wards (existing wards)
            {
                Ward ward = wards.getInfo(wardPtr);
                ward.display();
                wardPtr = wardPtr->next;
            }
        }

        // function to shift patients in the hospital
        void shift_patients() 
        {
            Node<GlobalIndex>* index_element = IndexBeds.pfirst;
            while (index_element != nullptr) // if this index_element exists
            {
                Bed* bed = index_element->info.bed;
                if (bed->isAvailable()) 
                {

                    Node<GlobalIndex> *next_index_element = index_element->next; // get the next_index_element
                    if (next_index_element != nullptr) 
                    {
                        Bed* next_bed = next_index_element->info.bed;
                        if (next_bed->isAvailable()) 
                        {
                            return;
                        }

                        bed->assignPatient(next_bed->getPatient()); // move patient from next bed to current bed
                        next_bed->dischargePatient();
                    }
                }
                index_element = index_element->next;
            }
        }

        // function to add a new ward
        Ward* addWard(int numBeds) 
        {
            wardCount++;
            Ward* newWard = new Ward(wardCount, numBeds, IndexBeds);
            wards.addLast(*newWard);
            return newWard;
        }

        // function to admit a patient
        void admitPatient(std::string patient_name, std::string cond) 
        {
            Node<Bed>* bedPtr;
            Patient* new_patient = new Patient(patient_name, cond);
            Node<Ward>* wardPtr = wards.pfirst;

            while (wardPtr != nullptr) { // check if there are available beds in existing wards
                Ward ward = wards.getInfo(wardPtr);
                bedPtr = ward.assignPatientToBed(new_patient);
                if (bedPtr != nullptr) 
                {
                    std::cout << "Patient " << patient_name << " admitted to Ward №" << ward.wardIndex << std::endl;
                    return;
                }
                wardPtr = wardPtr->next;
            }

            if (bedPtr == nullptr) // if no available beds in all wards, add a new ward
            {
                Ward* new_ward = addWard(4); 
                bedPtr = new_ward->assignPatientToBed(new_patient);
                if (bedPtr != nullptr) 
                {
                    std::cout << "Patient " << patient_name << " admitted to new Ward №" << new_ward->wardIndex << std::endl;
                    return;
                }
            }   
        }


        // function to get patient info by bed global index
        std::string getPatientInfo(int bedIndex) 
        {
            Node<GlobalIndex>* index_element = IndexBeds.pfirst;
            while (index_element != nullptr) // iterate through the global index list until we find the bed with the given index
            {
                int index = index_element->info.index;
                if (index == bedIndex) 
                {
                    Bed* bed = index_element->info.bed;
                    if (bed != nullptr && !bed->isAvailable()) { // if bed exists and is occupied
                        return bed->getPatient()->name + " - " + bed->getPatient()->condition;
                    }

                    else 
                    {
                        return "Bed is empty.";
                    }
                }
                index_element = index_element->next;
            }
            return "Bed not found.";
        }

        // function to discharge a patient from hospital by bed global index
        void dischargePatient(int bedIndex) 
        {
            Node<GlobalIndex>* index_element = IndexBeds.pfirst;

            // find the bed with the given index 
            while (index_element != nullptr) 
            {
                int index = index_element->info.index;
                if (index == bedIndex) 
                {
                    Bed* bed = index_element->info.bed;
                    if (bed != nullptr && !bed->isAvailable()) // if bed exists and is occupied - discharge the patient  and shift other patients
                    {
                        bed->dischargePatient();
                        shift_patients();
                        std::cout << "Patient discharged from bed " << bedIndex << std::endl;
                    } 
                    else {
                        std::cout << "Bed is already empty." << std::endl;
                    }
                    return;
                }
                index_element = index_element->next;
            }
            std::cout << "Bed not found." << std::endl;
        }

        // function to transfer a patient from one bed to another
        void transferPatient(int sourceBedIndex, int targetBedIndex) 
        {
            Bed *from_bed = nullptr;
            Bed *to_bed = nullptr;
            Bed *first_empty_bed = nullptr;
            Patient* patient;
            Node<GlobalIndex>* source_index_element = IndexBeds.pfirst;
            Node<GlobalIndex>* target_index_element = IndexBeds.pfirst;

            // find the source bed and check if the source bed is occupied
            while (source_index_element != nullptr) 
            {
                int index = source_index_element->info.index;
                if (index == sourceBedIndex) 
                {
                    from_bed = source_index_element->info.bed;
                    if (from_bed != nullptr && !from_bed->isAvailable()) // if this source bed exists and is occupied
                    {
                        // get the patient from the source bed
                        patient = from_bed->getPatient();
                        break;
                    }

                    else 
                    {
                        std::cout << "Source bed is empty. Transfer is not possible." << std::endl;
                        return;
                    }
                }
                source_index_element = source_index_element->next;
            }

            if (from_bed == nullptr)
            {
                std::cout << "Source bed is not found. Transfer is not possible." << std::endl;
                return;
            }

            
            while (target_index_element != nullptr) // find the target bed and check if the target bed is available
            {
                int index = target_index_element->info.index;
                Bed* bed = target_index_element->info.bed;

                if (bed != nullptr && bed->isAvailable()) // if bed exists and is available - it will be first empty bed 
                // (a patient will be transfered directly there in order to maintain non-empty bed requirement)
                { 
                    if (first_empty_bed == nullptr) 
                    {
                        first_empty_bed = bed;
                    }
                }

                if (index == targetBedIndex) 
                {
                    to_bed = target_index_element->info.bed;

                    if (to_bed != nullptr && to_bed->isAvailable()) 
                    {
                        // found the target bed
                        break;
                    } 

                    else 
                    {
                        std::cout << "Target bed is occupied. Transfer is not possible." << std::endl;
                        return;
                    }
                }
                target_index_element = target_index_element->next;
            }
            if (to_bed == nullptr) 
            {
                std::cout << "Target bed is not found. Transfer is not possible." << std::endl;
                return;
            }

            // discharge the patient from the source bed and assign the patient to the first empty bed
            from_bed->dischargePatient();
            first_empty_bed->assignPatient(patient);

            shift_patients(); // then shift all other patients in the hospital
        }
};

int main() {
    hospitalManagementSystem hospital;

    hospital.admitPatient("Eminem", "Flu");
    hospital.admitPatient("Peter Parker", "Cold");
    hospital.admitPatient("Ariana Grande", "Virus");
    hospital.admitPatient("Mykhailo Mudryk", "Doping");
    hospital.admitPatient("Andrei", "Broken leg");
    hospital.admitPatient("Marta", "Stomach pain");
    hospital.admitPatient("Artem", "Headache");

    std::cout << std::endl;
    hospital.displaySystem();

    std::cout << std::endl;

    std::cout << hospital.getPatientInfo(0) << std::endl;
    std::cout << hospital.getPatientInfo(4) << std::endl;
    std::cout << hospital.getPatientInfo(8) << std::endl;

    std::cout << std::endl;
    hospital.dischargePatient(0);
    std::cout << hospital.getPatientInfo(0) << std::endl;

    hospital.displaySystem();


    std::cout << std::endl;
    hospital.transferPatient(1, 2);
    hospital.transferPatient(1, 7);
    hospital.displaySystem();

    std::cout << std::endl;
    hospital.transferPatient(1, 9);
    hospital.transferPatient(2, 6);
    hospital.displaySystem();

    std::cout << std::endl;
    hospital.transferPatient(0, 7);
    hospital.displaySystem();


    return 0;
}