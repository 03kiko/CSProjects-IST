package xxl.cells;

import java.io.Serializable;
import java.util.LinkedList;
import java.util.List;
import xxl.cells.renderers.Renderer;
import xxl.cells.renderers.Renderable;
import xxl.cells.search.Search;
import xxl.cells.search.Searchable;
import xxl.storage.CellStorage;

/**
 * This class represents a cell in a Spreadsheet.
 * It can hold literals, binary functions and functions over a linear
 * range. It can also be observed.
 */
public class Cell implements Serializable, CellSubject, CellObserver, Renderable, Searchable {
	/**
	 * @return integer content of the cell; null if it has no integer
	 */
	public Integer getInteger() {
		if (isEmpty())
			return null;
		return _content.getInteger();
	}

	/**
	 * @return string content of the cell; null if it has no string
	 */
	public String getString() {
		if (isEmpty())
			return null;
		return _content.getString();
	}

	/**
	 * Inserts new content in the cell
	 * 
	 * @param content to be inserted.
	 */
	public void insert(Content content) {
		if (!isEmpty())
			_content.destroy(this);
		_content = content;
		_content.initialize(this);
		update();
	}

	/**
	 * Deletes the content of a cell.
	 */
	public void delete() {
		if (isEmpty())
			return;
		_content.destroy(this);
		_content = null;
		notifyObservers();
	}

	/**
	 * Checks if a cell is no longer being used.
	 *
	 * @return true if it's no longer being used; false otherwise
	 */
	public boolean deletable() {
		return isEmpty() && _observers.isEmpty();
	}

	/** @See xxl.cells.renderers.Renderable#render */
	public String render(Renderer renderer) {
		if (isEmpty())
			return "";
		return _content.render(renderer);
	}

	public boolean ok(Search search) {
		if(isEmpty())
			return false;
		return _content.ok(search);
	}

	/**
	 * Copies this cell to another storage.
	 * 
	 * @param dest storage to copy to.
	 * @param pos position to copy to.
	 * @param dependencies should it copy this cell's dependencies?
	 */
	public void copyTo(CellStorage dest, Position pos, boolean dependencies) {
		if (isEmpty())
			return;
		_content.copyTo(dest, pos, dependencies);
	}

	/**
	 * Adds an observer that will be notified of changes
	 * 
	 * @param observer to be added
	 */
	@Override
	public void addObserver(CellObserver observer) {
		if (!_observers.contains(observer))
			_observers.add(observer);
	}

	/**
	 * Removes an observer so it is no longer notified
	 * 
	 * @param observer to be removed
	 */
	@Override
	public void removeObserver(CellObserver observer) {
		_observers.remove(observer);
	}

	/** Notifies all observers a change occured */
	@Override
	public void notifyObservers() {
		for (CellObserver observer : _observers)
			observer.update();
	}

	/**
	 * Updates everything observing this, when this is updated.
	 */
	@Override
	public void update() {
		_content.update();
		notifyObservers();
	}

	/** @return true if content is null; false otherwise */
	private boolean isEmpty() {
		return _content == null;
	}

	/** By default, a cell has no content */
	private Content _content = null;

	/** Observers "looking at" this cell */
	private List<CellObserver> _observers = new LinkedList<>();

}
