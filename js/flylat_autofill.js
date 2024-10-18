// ==UserScript==
// @name         Flylat Form Autofill with Random Aircraft
// @namespace    http://tampermonkey.net/
// @version      0.9
// @description  Autofills the form with fleet data from localStorage and allows random selection of parked aircraft
// @author       You
// @match        https://app.flylat.net/hire_ai
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

    var aircraftselection;

    const button = document.createElement('button');
    button.innerText = 'Random';
    button.className = 'btn btn-cancel'; // Hier kannst du eine eigene Klasse für Styling hinzufügen
    button.style.backgroundColor = '#28a745'; // Optional: Button-Farben anpassen
    button.style.color = 'white';
    button.style.border = 'none';
    button.style.padding = '10px 20px';
    button.style.borderRadius = '5px';

    button.onclick = function() {
        selectRandomAircraft();
    };

    // Finde die bestehende <div> mit den beiden Buttons
    const buttonContainer = document.querySelector('.d-flex.justify-content-between');

    // Füge den neuen Button zwischen die bestehenden Buttons ein
    if (buttonContainer) {
        const cancelButton = buttonContainer.querySelector('.btn-cancel');
        const createButton = buttonContainer.querySelector('.btn-create');

        // Füge den neuen Button vor dem "Create"-Button ein
        buttonContainer.insertBefore(button, createButton);
    }

    const form = document.querySelector('form');

    if (form) {
        form.addEventListener('submit', (event) => {
            setTimeout(() => {
                console.log("Entferne Flugzeug:", aircraftselection);
                removeAircraftFromStorage(aircraftselection); // Entferne das Flugzeug nach dem Absenden
                window.open('https://app.flylat.net/hire_ai', '_blank');
            }, 100);
        });
    }

    const cardTitle = document.querySelector('h5.card-title.text-center');
    const card = document.querySelector(".card");

    const buttonfleetContainer = document.createElement('div');
    buttonfleetContainer.className = 'd-flex justify-content-between align-items-center w-100';

    const titleContainer = document.createElement('div');
    titleContainer.className = 'flex-grow-1 d-flex justify-content-center';
    titleContainer.appendChild(cardTitle);

    const fleetButton = document.createElement('button');
    fleetButton.innerText = 'Fleet';
    fleetButton.className = 'btn btn-primary';
    fleetButton.style.padding = '5px'
    fleetButton.onclick = function() {
        window.location.href = 'https://app.flylat.net/fleet';
    };

    buttonfleetContainer.appendChild(titleContainer);
    buttonfleetContainer.appendChild(fleetButton);

    if (card) {
        card.insertBefore(buttonfleetContainer, card.firstChild);
    }

    function uploadDataToForm(aircraft) {
        console.log("Selected Aircraft:", aircraft);

        if (!aircraft) {
            console.warn("No aircraft data provided.");
            return;
        }

        fillFormFields(aircraft);
    }

    function fillFormFields(aircraft) {
        const nameInput = document.querySelector('#crewName');
        const departureSelect = document.querySelector('#departure');
        const destinationSelect = document.querySelector('#destination');
        const aircraftSelect = document.querySelector('#aircraft');

        if (nameInput) {
            nameInput.value = aircraft.route || "";
        }

        console.log("Filling fields with:", {
            route: aircraft.route,
            departure: aircraft.departure,
            destination: aircraft.destination,
            aircraftName: aircraft.name
        });


        if (departureSelect && departureSelect.options.length > 0) {
            departureSelect.value = aircraft.departure || ""; // Departure ICAO
            const departureEvent = new Event('change', { bubbles: true });
            departureSelect.dispatchEvent(departureEvent);
        }
        setTimeout(() => {
            if (destinationSelect) {
                destinationSelect.value = aircraft.destination || ""; // Destination ICAO
                // Trigger für Änderung
                const destinationEvent = new Event('change', { bubbles: true });
                destinationSelect.dispatchEvent(destinationEvent);
            }
            setTimeout(100);
            if (aircraftSelect) {
                // Suche die Option mit dem passenden Namen
                const aircraftValue = aircraft.name; // z.B. OE-ENC
                const options = aircraftSelect.options;

                for (let i = 0; i < options.length; i++) {
                    // Überprüfe, ob die Option den gesuchten Namen enthält
                    if (options[i].text.includes(aircraftValue)) {
                        aircraftSelect.value = options[i].value; // Setze den Wert auf den entsprechenden Wert
                        // Trigger für Änderung
                        const aircraftEvent = new Event('change', { bubbles: true });
                        aircraftSelect.dispatchEvent(aircraftEvent);
                        break; // Breche die Schleife ab, nachdem die Option gefunden wurde
                    }
                }
            }
        }, 350);
    }

    function selectRandomAircraft() {
        const fleetData = JSON.parse(localStorage.getItem('AIFleet') || '[]');
        console.log("AIFleet Data:", fleetData);

        if (fleetData.length === 0) {
            console.warn("No available parked aircraft in localStorage.");
            return;
        }

        const randomIndex = Math.floor(Math.random() * fleetData.length);
        const selectedAircraft = fleetData[randomIndex];

        aircraftselection = selectedAircraft.name;
        uploadDataToForm(selectedAircraft);
    }

    function removeAircraftFromStorage(selectedAircraftName) {
        const fleetData = JSON.parse(localStorage.getItem('AIFleet') || '[]');
        if (fleetData.length === 0) return;

        // Entferne das Flugzeug
        const updatedFleetData = fleetData.filter(aircraft => aircraft.name !== selectedAircraftName);
        localStorage.setItem('AIFleet', JSON.stringify(updatedFleetData));
        console.log("Updated AIFleet after removal:", updatedFleetData);
    }

    function getAIFleet() {

        const fleetData = JSON.parse(localStorage.getItem('fleetData') || '[]');
        if (fleetData.length === 0) {
            console.log("No fleetData in localStorage.");
            return;
        }

        console.log("Fleet Data:", fleetData);

        const availableAircraft = fleetData.filter(aircraft =>
                                                   aircraft.occupied == "AI" &&
                                                   aircraft.status == "PARKED" &&
                                                   aircraft.hub == aircraft.location
                                                  );

        console.log("Available Aircraft:", availableAircraft);

        button.disabled = false;
        localStorage.removeItem('AIFleet');
        localStorage.setItem('AIFleet', JSON.stringify(availableAircraft));
        localStorage.removeItem('timeStamp');
        localStorage.removeItem('fleetData');
        console.log("AIFleet stored:", JSON.stringify(availableAircraft));

        button.style.backgroundColor = '#28a745';

        if (availableAircraft.length === 0) {
            console.warn("No available parked aircraft in localStorage.");
            button.style.backgroundColor = '';
            button.disabled = true;
            localStorage.removeItem('AIFleet');
            return;
        }
    }

    function init() {
        getAIFleet();
        selectRandomAircraft();
    }

    getAIFleet();

    if (localStorage.getItem('AIFleet').length === 0) {
        console.warn("No available parked aircraft in localStorage.");
        button.style.backgroundColor = '';
        button.disabled = true;
        localStorage.removeItem('AIFleet');
        return;
    }

    window.addEventListener('DOMContentLoaded', init);

})();
