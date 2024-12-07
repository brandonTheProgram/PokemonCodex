const express = require('express');
const expressLayouts = require('express-ejs-layouts');
const path = require('path');
const axios = require('axios');

const app = express();
const port = 3000;

// Set EJS as the view engine
app.set('view engine', 'ejs');

// Set the views directory to the current folder
app.set('views', path.join(__dirname));

// Use express-ejs-layouts for layout management
app.use(expressLayouts);

// Set the layout file, assuming 'main.ejs' is in the 'layouts' folder
app.set('layout', path.join(__dirname, 'layouts', 'main.ejs'));

app.use(express.json());

// Serve static files from the "public" folder (icons, css, etc.)
app.use('/css', express.static(path.join(__dirname, 'public', 'css')));
app.use('/icons', express.static(path.join(__dirname, 'public', 'icons')));

// Render the Index.ejs
app.get('/', async (req, res) => {
    const region = req.params.region;

    try {        
        // Send request to C++ backend (httplib) on port 8080
        const response = await axios.get(`http://localhost:8080/GetRegionNames`);

        // Get the data from the backend response
        const regionData = response.data;

        // Render Index.ejs
        res.render('Index', {
            regions: regionData
        });
    } catch (error) {
        console.error('Error fetching data from backend:', error);
        res.status(500).send('Internal Server Error');
    }
});

// Serve the Region file
app.get('/region/:region', async (req, res) => {
    const region = req.params.region;

    try {        
        // Send request to C++ backend (httplib) on port 8080
        const response = await axios.get(`http://localhost:8080/region/${region}`);

        // Get the data from the backend response
        const pokemonData = response.data;

        // Render Region.ejs
        res.render('Region', {
            region: region,
            pokemons: pokemonData 
        });
    } catch (error) {
        console.error('Error fetching data from backend:', error);
        res.status(500).send('Internal Server Error');
    }
});

app.get('/pokemon/:pokedex_number/:region_id?', async (req, res) => {
    const pokedexNumber = req.params.pokedex_number;
    const regionId = req.params.region_id; // region_id is optional

    try {

    } catch (error) {
    console.error('Error fetching data from backend:', error);
    res.status(500).send('Internal Server Error');
    }
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
