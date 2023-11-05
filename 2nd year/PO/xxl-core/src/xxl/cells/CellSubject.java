package xxl.cells;

/**
 * This interface represents a Cell that can be observed.
 */
public interface CellSubject {
	/**
	 * Adds an observer to a subject
	 * @param observer to be added
	 */
	void addObserver(CellObserver observer);
	
	/**
	 * Removes an observer from a subject
	 * @param observer to be removed
	 */
	void removeObserver(CellObserver observer);

	/** Notifies all observers a change occured */
	void notifyObservers();
}
