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
    button.className = 'btn btn-cancel';
    button.style.backgroundColor = '#28a745';
    button.style.color = 'white';
    button.style.border = 'none';
    button.style.padding = '10px 20px';
    button.style.borderRadius = '5px';

    button.onclick = function() {
        selectRandomAircraft();
    };

    const buttonContainer = document.querySelector('.d-flex.justify-content-between');
    if (buttonContainer) {
        const cancelButton = buttonContainer.querySelector('.btn-cancel');
        const createButton = buttonContainer.querySelector('.btn-create');
        buttonContainer.insertBefore(button, createButton);
    }

    const form = document.querySelector('form');
    if (form) {
        form.addEventListener('submit', (event) => {
            setTimeout(() => {
                console.log("Entferne Flugzeug:", aircraftselection);
                removeAircraftFromStorage(aircraftselection);
                window.open('https://app.flylat.net/hire_ai', '_blank');
            }, 100);
        });
    }

    const cardTitle = document.querySelector('h5.card-title.text-center');
    const card = document.querySelector(".card");

    const buttonFleetContainer = document.createElement('div');
    buttonFleetContainer.className = 'd-flex justify-content-between align-items-center w-100';

    const titleContainer = document.createElement('div');
    titleContainer.className = 'flex-grow-1 d-flex justify-content-center';
    titleContainer.appendChild(cardTitle);

    const fleetButton = document.createElement('button');
    fleetButton.innerText = 'Fleet';
    fleetButton.className = 'btn btn-primary';
    fleetButton.style.padding = '5px';
    fleetButton.onclick = function() {
        window.location.href = 'https://app.flylat.net/fleet';
    };

    buttonFleetContainer.appendChild(titleContainer);
    buttonFleetContainer.appendChild(fleetButton);

    if (card) {
        card.insertBefore(buttonFleetContainer, card.firstChild);
    }

    const flightInfoBox = document.createElement('div');
    flightInfoBox.className = 'flight-info-box'; // Füge eine Klasse hinzu für das Styling
    flightInfoBox.style.display = 'none'; // Standardmäßig ausblenden
    flightInfoBox.style.padding = '10px';
    flightInfoBox.style.border = '1px solid #ccc';
    flightInfoBox.style.borderRadius = '5px';
    flightInfoBox.style.marginTop = '10px';


    if (card) {
        const firstH6 = card.querySelector('h6');
        if (firstH6) {
            firstH6.insertAdjacentElement('afterend', flightInfoBox);
        }
    }

    function uploadDataToForm(aircraft) {
        console.log("Selected Aircraft:", aircraft);
        if (!aircraft) {
            console.warn("No aircraft data provided.");
            return;
        }

        fillFormFields(aircraft);
        displayFlightInfo(aircraft); // Zeige die Fluginformationen an
    }

    function displayFlightInfo(aircraft) {
        flightInfoBox.innerHTML = `
            <div><strong>Departure:</strong> ${aircraft.departure || "N/A"}</div>
            <div><strong>Destination:</strong> ${aircraft.destination || "N/A"}</div>
            <div><strong>Aircraft Name:</strong> ${aircraft.name || "N/A"}</div>
        `;
        flightInfoBox.style.display = 'block'; // Zeige die Box an
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
            departureSelect.value = aircraft.departure || "";
            const departureEvent = new Event('change', { bubbles: true });
            departureSelect.dispatchEvent(departureEvent);
        }
        setTimeout(() => {
            if (destinationSelect) {
                destinationSelect.value = aircraft.destination || "";
                const destinationEvent = new Event('change', { bubbles: true });
                destinationSelect.dispatchEvent(destinationEvent);
            }
            setTimeout(100);
            if (aircraftSelect) {
                const aircraftValue = aircraft.name;
                const options = aircraftSelect.options;

                for (let i = 0; i < options.length; i++) {
                    if (options[i].text.includes(aircraftValue)) {
                        aircraftSelect.value = options[i].value;
                        const aircraftEvent = new Event('change', { bubbles: true });
                        aircraftSelect.dispatchEvent(aircraftEvent);
                        break;
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
