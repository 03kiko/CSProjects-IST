package xxl.app.search;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.Spreadsheet;
import xxl.app.renderers.OutputRenderer;
import xxl.cells.search.PositionCellPair;
import java.util.List;
import xxl.exceptions.UnknownInputException;

/**
 * Command for searching content values.
 */
class DoShowValues extends Command<Spreadsheet> {

	DoShowValues(Spreadsheet receiver) {
		super(Label.SEARCH_VALUES, receiver);
		addStringField("value", Prompt.searchValue());
	}

	@Override
	protected final void execute() throws CommandException {
		try{
			OutputRenderer renderer = new OutputRenderer();
			List<PositionCellPair> ok = _receiver.search(new ValueSearch(stringField("value")));
			for(PositionCellPair pair: ok) {
				_display.addLine(renderer.combine(pair.getPosition(), pair.getCell()));
			}
			_display.display();
		} catch(UnknownInputException e) { return; }
	}
}
