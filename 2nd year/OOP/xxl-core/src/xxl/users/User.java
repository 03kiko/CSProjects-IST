package xxl.users;

import java.util.ArrayList;
import java.util.List;
import java.io.Serializable;

import xxl.Spreadsheet;

/**
 * This class represents an user of the application.
 * A user can own multiple spreadsheets.
 */
public class User implements Serializable {
	/**
	 * @param name name of the user.
	 */
	public User(String name) { _name = name; }

	/**
	 * Adds a new spreadsheet to this user.
	 *
	 * @param spreadsheet to add.
	 */
	public void add(Spreadsheet spreadsheet) {
		if (!owns(spreadsheet))
			_owned.add(spreadsheet);
	}

	/**
	 * Removes a spreadsheet from this user.
	 *
	 * @param spreadsheet to remove.
	 */
	public void remove(Spreadsheet spreadsheet) {
		_owned.remove(spreadsheet);
	}

	/**
	 * Checks if the user owns a spreadsheet.
	 *
	 * @param spreadsheet to search for.
	 * @return true if the user owns it; false otherwise.
	 */
	public boolean owns(Spreadsheet spreadsheet) {
		return _owned.contains(spreadsheet);
	}

	/** @return the name of this user. */
	public String getName() { return _name; }

	private String _name;
	private List<Spreadsheet> _owned = new ArrayList<>();
}
