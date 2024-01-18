import fs from "fs";
import path from "path";

// Definiendo los directorios
const componentsDir = path.join(process.cwd(), 'src/components');
const storiesDir = path.join(process.cwd(), 'src/stories');

// Crear el directorio de historias si no existe
if (!fs.existsSync(storiesDir)){
    fs.mkdirSync(storiesDir);
}

// Leer todos los archivos de componentes
fs.readdir(componentsDir, (err, files) => {
    if (err) {
        console.error('Error al leer el directorio de componentes:', err);
        return;
    }

    files.forEach(file => {
        // Asumiendo que los archivos de componentes terminan en .jsx
        if (file.endsWith('.pilo.jsx')) {
            const componentName = file.replace('.pilo.jsx', '');
            const storyFilePath = path.join(storiesDir, `${componentName}.js`);

            const content = `
import React from 'react';
import ${componentName} from '../components/${file}';
import MySBForReact from '../components/MySBForReact.jsx';

const ${componentName}Story = () => (
	<MySBForReact>
		<${componentName} />
	</MySBForReact>
);

export default ${componentName}Story;
`;

            // Escribir el archivo Astro
            fs.writeFile(storyFilePath, content, err => {
                if (err) {
                    console.error('Error al escribir el archivo de historia:', err);
                } else {
                    console.log(`Historia generada: ${storyFilePath}`);
                }
            });
        }
    });
});
