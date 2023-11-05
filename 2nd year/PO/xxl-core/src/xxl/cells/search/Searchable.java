package xxl.cells.search;

/**
 * This interface is used to represent all searchable objects.
 */
public interface Searchable {
	/**
	 * Accepts the visitor for conducting a search.
	 *
	 * @param search The search criteria to be aplied.
	 * @return true if the object meets the search criteria; false otherwise.
	 */
	boolean ok(Search search);
}

