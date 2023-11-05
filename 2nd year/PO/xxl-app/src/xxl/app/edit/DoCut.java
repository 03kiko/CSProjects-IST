package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.Spreadsheet;
import xxl.exceptions.RangeException;

/**
 * Cut command.
 */
class DoCut extends Command<Spreadsheet> {

	DoCut(Spreadsheet receiver) {
		super(Label.CUT, receiver);
		addStringField("range", Prompt.address());
	}

	@Override
	protected final void execute() throws CommandException {
		try {
			_receiver.cut(stringField("range"));
		} catch (RangeException e) {
			throw new InvalidCellRangeException(e.getMessage());
		}
	}

}
