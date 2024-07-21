package xxl;

import java.io.IOException;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.ObjectOutputStream;
import java.io.BufferedOutputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.BufferedInputStream;
import java.io.FileInputStream;

import xxl.parser.Parser;
import xxl.exceptions.ImportFileException;
import xxl.exceptions.MissingFileAssociationException;
import xxl.exceptions.UnavailableFileException;
import xxl.exceptions.UnrecognizedEntryException;
import xxl.exceptions.UnknownInputException;
import xxl.exceptions.RangeException;
import xxl.cells.Position;
import xxl.users.User;

/** 
 * Class representing a spreadsheet application.
 */
public class Calculator {
	/**
	 * Creates a new anonymous spreadsheet.
	 * 
	 * @param rows number of rows
	 * @param columns number of columns
	 */
	public void create(int rows, int columns) {
		_filename = null;
		_spreadsheet = new Spreadsheet(rows, columns, _activeUser);
	}

	/**
	* Saves the serialized application's state into the file associated to the current network.
	*
	* @throws FileNotFoundException if for some reason the file cannot be created or opened. 
	* @throws MissingFileAssociationException if the current network does not have a file.
	* @throws IOException if there is some error while serializing the state of the network to disk.
	*/
	public void save() throws FileNotFoundException, MissingFileAssociationException, IOException {
		if (_filename == null)
			throw new MissingFileAssociationException();
		try (var out = new ObjectOutputStream(new BufferedOutputStream(new FileOutputStream(_filename)))) {
			out.writeObject(_spreadsheet);
			_spreadsheet.savedChanges();	
		} catch (IOException e) { throw e; } // ensure auto-closeable
	}

	/**
	* Saves the serialized application's state into the specified file.
	* The current network is associated to this file.
	*
	* @param filename the name of the file.
	* @throws FileNotFoundException if for some reason the file cannot be created or opened.
	* @throws IOException if there is some error while serializing the state of the network to disk.
	*/
	public void saveAs(String filename) throws FileNotFoundException, IOException {
		_filename = filename;
		try {
			save();
		} catch (MissingFileAssociationException e) {} // ghost exception
	}

	/**
	* @param filename name of the file containing the serialized application's state to load.
	* @throws UnavailableFileException if the specified file does not exist or there is an error while processing this file.
	*/
	public void load(String filename) throws UnavailableFileException {
		try (var in = new ObjectInputStream(new BufferedInputStream(new FileInputStream(filename)))) {
			_spreadsheet = (Spreadsheet) in.readObject();
			_spreadsheet.savedChanges();
			_filename = filename;
		} catch (ClassNotFoundException | IOException e) {
			throw new UnavailableFileException(filename);
		}
	}

	/**
	* Read text input file and create domain entities.
	*
	* @param filename name of the text input file.
	* @throws ImportFileException when the file cannot be properly read.
	*/
	public void importFile(String filename) throws ImportFileException {
		try(BufferedReader reader = new BufferedReader(new FileReader(filename))) {
			String line = "";
			try {
				Parser parser = new Parser();
				Position size = new Position(-1, -1);
				while (size.getRow() < 0 || size.getColumn() < 0) {
					line = reader.readLine();
					if (line == null)
						break;
					parser.readSize(line, size);
				}
				_spreadsheet = new Spreadsheet(size.getRow(), size.getColumn(), _activeUser);
				while((line = reader.readLine()) != null) {
					String[] split = parser.splitEntry(line);
					if (split.length == 1)
						continue; //ignore empty positions
					_spreadsheet.insertContents(split[0], split[1]);
				}
			} catch (UnknownInputException | RangeException internalExceptions)  {
				throw new UnrecognizedEntryException(line);
			}
		} catch (IOException | UnrecognizedEntryException e) {
			throw new ImportFileException(filename, e);
		}
	}

	/**
	 * Returns the current spreadsheet.
	 *
	 * @return the spreadsheet.
	 */
	public Spreadsheet getSpreadsheet() {
		return _spreadsheet;
	}

	/**
	 * Checks if there are unsaved changes.
	 *
	 * @return true if so; false otherwise.
	 */
	public boolean unsavedChanges() {
		return (_spreadsheet != null) && _spreadsheet.wasChanged();
	}

	/** Name of the file associated to the current spreadsheet. */
	private String _filename = null;

	/** The currently associated spreadsheet. */
	private Spreadsheet _spreadsheet = null;

	/** The currently active user */
	private User _activeUser = new User("root");
}
