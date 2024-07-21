package xxl;

import java.io.Serial;
import java.io.Serializable;
import java.util.TreeMap;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;

import xxl.exceptions.RangeException;
import xxl.exceptions.UnknownInputException;
import xxl.exceptions.RangeOutOfBoundsException;
import xxl.cells.Position;
import xxl.cells.Cell;
import xxl.cells.Content;
import xxl.storage.*;
import xxl.cells.Interval;
import xxl.cells.renderers.Renderer;
import xxl.users.User;
import xxl.parser.Parser;
import xxl.cutBuffer.CutBuffer;
import xxl.cells.search.*;
/**
 * Class representing a spreadsheet.
 */
public class Spreadsheet implements Serializable {

	@Serial
	private static final long serialVersionUID = 202308312359L;

	/**
	 * Initializes a new spreadsheet linked to the user.
	 *
	 * @param rows number of rows.
	 * @param columns number of columns.
	 * @param user user to link the spreadsheet to.
	 */
	public Spreadsheet(int rows, int columns, User user) {
		_storage = new TreeStorage(rows, columns);
		_cutBuffer = new CutBuffer(_storage);
		addUser(user);
	}

	/**
	* Insert specified content in specified range.
	*
	* @param rangeSpecification interval to insert in.
	* @param contentSpecification content to insert.
	*
	* @throws RangeException when an illegal interval is used.
	* @throws UnknownInputException when one of the inputs cannot be parsed.
	*/
	public void insertContents(String rangeSpecification, String contentSpecification)
			throws RangeException, UnknownInputException {
		Content content = _parser.readContent(this, contentSpecification);
		Interval interval = _parser.readInterval(rangeSpecification);
		if (!_storage.inBounds(interval))
			throw new RangeOutOfBoundsException(rangeSpecification);
		for (Position pos : interval)
			_storage.insert(pos, content);
		_unsavedChanges = true;
	}

	/**
	 * View the contents of the spreadsheet.
	 *
	 * @param rangeSpecification range to view.
	 * @param renderer how to render.
	 * @return "visual" representation of the contents.
	 *
	 * @throws RangeException when the range is not linear
	 * 		or out of bounds.
	 * @throws UnknownInputException when the range cannot be parsed.
	 */
	public String view(String rangeSpecification, Renderer renderer)
			throws RangeException, UnknownInputException {
		Interval interval = _parser.readInterval(rangeSpecification);
		String result = "";
		if (!_storage.inBounds(interval))
			throw new RangeOutOfBoundsException(rangeSpecification);
		boolean first = true;
		for (Position pos : interval) {
			if (!first)
			    result += "\n";
			else
			    first = false;
			result += renderer.combine(pos, _storage.view(pos));
		}
		return result;
	}

	public void delete(String rangeSpecification) throws RangeException {
		Interval interval = _parser.readInterval(rangeSpecification);
		if (!_storage.inBounds(interval))
			throw new RangeOutOfBoundsException(rangeSpecification);
		for (Position pos : interval)
			_storage.delete(pos);
		_unsavedChanges = true;
	}

	public void copy(String rangeSpecification) throws RangeException {
		Interval interval = _parser.readInterval(rangeSpecification);
		if (!_storage.inBounds(interval))
			throw new RangeOutOfBoundsException(rangeSpecification);
		_cutBuffer.copy(interval, _storage);
		_unsavedChanges = true;
	}

	public void paste(String rangeSpecification) throws RangeException {
		Interval interval = _parser.readInterval(rangeSpecification);
		if (!_storage.inBounds(interval))
			throw new RangeOutOfBoundsException(rangeSpecification);
		_cutBuffer.paste(interval, _storage);
		_unsavedChanges = true;
	}

	public void cut(String rangeSpecification) throws RangeException {
		copy(rangeSpecification);
		delete(rangeSpecification);
	}

	public String showCutBuffer(Renderer renderer) {
		return _cutBuffer.view(renderer);
	}

	public List<PositionCellPair> search(Search search) {
		Interval interval = _storage.getInterval();
		List<PositionCellPair> pairs = new ArrayList<>();
		for(Position pos: interval) {
			Cell cell = _storage.view(pos);
			if(cell == null)
				continue;
			if(cell.ok(search)) {
				PositionCellPair pair = new PositionCellPair(new Position(pos.getRow(), pos.getColumn()), cell);
				pairs.add(pair);
			}
		}

		return pairs;
	}

	/**
	 * Confirms all changes made were saved.
	 */
	public void savedChanges() { _unsavedChanges = false; } 

	/**
	 * Checks if there are unsaved changes.
	 */
	public boolean wasChanged() { return _unsavedChanges; }

	/**
	 * Gets the storage of this spreadsheet.
	 *
	 * @return the storage.
	 */
	public CellStorage getStorage() { return _storage; }

	/**
	 * Adds a new user to this spreadsheet.
	 *
	 * @param user user to add.
	 */
	public void addUser(User user) { _owners.put(user.getName(), user); }


	private CellStorage _storage;
	private Map<String, User> _owners = new TreeMap<>();
	private CutBuffer _cutBuffer;
	private Parser _parser = new Parser();
	private boolean _unsavedChanges = true;
}
